#include "terminal.h"

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

/* Gracias a MagicalBat */

typedef struct {
    HANDLE stdinHandle;
    HANDLE stdoutHandle;
    DWORD modoInputOriginal;
    DWORD modoOutputOriginal;
} tWin32TermBackend;

int terminalCrear(tTerminal *term)
{
    tWin32TermBackend *backend;
    HANDLE stdinHandle, stdoutHandle;
    DWORD modoOriginal, nuevoModo;

    if(!term) {
        return ERR;
    }
    
    backend = malloc(sizeof(tWin32TermBackend));

    if(!backend) {
        return ERR;
    }

    stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if(stdinHandle == INVALID_HANDLE_VALUE || stdoutHandle == INVALID_HANDLE_VALUE) {
        return ERR;
    }

    backend->stdinHandle = stdinHandle;
    backend->stdoutHandle = stdoutHandle;

    term->backend = backend;

    return OK;
}

void terminalDestruir(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    free(term->backend);
    term->backend = NULL;
}

int terminalEntrarModoCrudo(tTerminal *term)
{
    tWin32TermBackend *backend;
    HANDLE stdinHandle, stdoutHandle;
    DWORD modoOriginal, nuevoModo;

    if(!term || !term->backend) {
        return ERR;
    }

    backend = term->backend;
    stdinHandle = backend->stdinHandle;
    stdoutHandle = backend->stdoutHandle;

    backend->modoInputOriginal = 0;
    if (GetConsoleMode(stdinHandle, &backend->modoInputOriginal) == 0) {
        return ERR;
    }

    nuevoModo = backend->modoInputOriginal;
    nuevoModo &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
    nuevoModo |= ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT;

    if (SetConsoleMode(stdinHandle, nuevoModo) == 0) {
        return ERR;
    }

    backend->modoOutputOriginal = 0;
    if(GetConsoleMode(stdoutHandle, &backend->modoOutputOriginal) == 0) {
        return ERR;
    }

    nuevoModo = backend->modoOutputOriginal;
    nuevoModo |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if(SetConsoleMode(stdoutHandle, nuevoModo) == 0) {
        return ERR;
    }

    
    return OK;
}

void terminalSalirModoCrudo(tTerminal *term)
{
    tWin32TermBackend *backend;

    if(!term || !term->backend) {
        return;
    }

    backend = term->backend;

    SetConsoleMode(backend->stdinHandle, backend->modoInputOriginal);
    SetConsoleMode(backend->stdoutHandle, backend->modoOutputOriginal);
}

void terminalLimpiar(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }
}

void terminalMoverCursor(
    tTerminal *term,
    unsigned fila,
    unsigned columna
) {
    if(!term || !term->backend) {
        return;
    }

    char buffer[32];

    snprintf(buffer, sizeof(buffer), "\x1b[%u;%uH", fila + 1, columna + 1);
    fputs(buffer, stdout); /* Shouldn't it be stdout?*/ 
}

void terminalOcultarCursor(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[?25l", stdout); /* Shouldn't it be stdout?*/
}

void terminalMostrarCursor(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[?25h", term->backend->stdoutHandle); /* Shouldn't it be stdout?*/
}

void terminalColorTexto(
    tTerminal *term,
    eColor color
) {
    if(!term || !term->backend) {
        return;
    }

    const char *codigoColor;

    switch(color) {
        case COLOR_NEGRO: codigoColor = "\x1b[30m"; break;
        case COLOR_ROJO: codigoColor = "\x1b[31m"; break;
        case COLOR_VERDE: codigoColor = "\x1b[32m"; break;
        case COLOR_AMARILLO: codigoColor = "\x1b[33m"; break;
        case COLOR_AZUL: codigoColor = "\x1b[34m"; break;
        case COLOR_MAGENTA: codigoColor = "\x1b[35m"; break;
        case COLOR_CYAN: codigoColor = "\x1b[36m"; break;
        case COLOR_BLANCO: codigoColor = "\x1b[37m"; break;
    }

    fputs(codigoColor, term->backend->stdoutHandle);
}

void terminalColorFondo(
    tTerminal *term,
    eColor color
) {
    if(!term || !term->backend) {
        return;
    }

    const char *codigoColor;

    switch(color) {
        case COLOR_NEGRO: codigoColor = "\x1b[40m"; break;
        case COLOR_ROJO: codigoColor = "\x1b[41m"; break;
        case COLOR_VERDE: codigoColor = "\x1b[42m"; break;
        case COLOR_AMARILLO: codigoColor = "\x1b[43m"; break;
        case COLOR_AZUL: codigoColor = "\x1b[44m"; break;
        case COLOR_MAGENTA: codigoColor = "\x1b[45m"; break;
        case COLOR_CYAN: codigoColor = "\x1b[46m"; break;
        case COLOR_BLANCO: codigoColor = "\x1b[47m"; break;
    }

    fputs(codigoColor, term->backend->stdoutHandle);
}

void terminalRestablecerAtributos(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[0m", term->backend->stdoutHandle);
}

void terminalEscribir(
    tTerminal *term,
    const char *texto
);

int terminalLeerEvento(
    tTerminal *term,
    tEventoTecla *ev
);

void terminalObtenerTam(
    tTerminal *term,
    unsigned *filas,
    unsigned *columnas
);

void terminalActualizar(tTerminal *term);
