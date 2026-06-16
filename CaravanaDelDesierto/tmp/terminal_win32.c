#include "terminal.h"
#include "../common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

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
        free(backend);
        return ERR;
    }

    backend->stdinHandle = stdinHandle;
    backend->stdoutHandle = stdoutHandle;

    term->backend = backend;

    /* Inicializamos el buffer interno para dibujado */
    term->cap = 4096;
    term->len = 0;
    term->buffer = malloc(term->cap);
    if(!term->buffer) {
        free(backend);
        term->backend = NULL;
        return ERR;
    }

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
    /* Mueve el cursor a la posición inicial y limpia la pantalla */
    fputs("\x1b[H\x1b[2J", stdout);
    fflush(stdout);
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

    /* _GNU_SOURCE en Windows? */
    snprintf(buffer, sizeof(buffer), "\x1b[%u;%uH", fila + 1, columna + 1);
    fputs(buffer, stdout); 
}

void terminalOcultarCursor(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[?25l", stdout);
    fflush(stdout);
}

void terminalMostrarCursor(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[?25h", stdout);
    fflush(stdout);
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
        case COLOR_DEFAULT: codigoColor = "\x1b[39m"; break;
    }

    fputs(codigoColor, stdout);
    fflush(stdout);
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
        case COLOR_DEFAULT: codigoColor = "\x1b[49m"; break;
    }

    fputs(codigoColor, stdout);
}

void terminalRestablecerAtributos(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[0m", stdout);
}

void terminalEscribir(
    tTerminal *term,
    const char *texto
) {
    if(!term || !term->backend) {
        return;
    }

    /* Append to internal buffer if possible, otherwise fall back to stdout */
    size_t tlen = strlen(texto);
    if(term->buffer && term->len + tlen < term->cap) {
        memcpy(term->buffer + term->len, texto, tlen);
        term->len += (unsigned)tlen;
        return;
    }

    fputs(texto, stdout);
    fflush(stdout);
}

int terminalLeerEvento(
    tTerminal *term,
    tEventoTecla *ev
) {
    if(!term || !term->backend || !ev) {
        return ERR;
    }

    tWin32TermBackend *backend = term->backend;
    INPUT_RECORD rec;
    DWORD read = 0;

    while (1) {
        if (ReadConsoleInputA(backend->stdinHandle, &rec, 1, &read) == 0) {
            return ERR;
        }

        if (rec.EventType != KEY_EVENT) {
            continue;
        }

        KEY_EVENT_RECORD ker = rec.Event.KeyEvent;
        if (!ker.bKeyDown) {
            continue;
        }

        /* Printable ASCII character */
        if (ker.uChar.AsciiChar != 0) {
            ev->tipo = TECLA_CARACTER;
            ev->caracter = (char)ker.uChar.AsciiChar;
            return OK;
        }

        /* Special keys mapped by virtual-key code */
        switch (ker.wVirtualKeyCode) {
            case VK_RETURN: ev->tipo = TECLA_ENTER; return OK;
            case VK_ESCAPE: ev->tipo = TECLA_ESC; return OK;
            case VK_BACK: ev->tipo = TECLA_BACKSPACE; return OK;
            case VK_TAB: ev->tipo = TECLA_TAB; return OK;
            case VK_UP: ev->tipo = TECLA_ARRIBA; return OK;
            case VK_DOWN: ev->tipo = TECLA_ABAJO; return OK;
            case VK_LEFT: ev->tipo = TECLA_IZQUIERDA; return OK;
            case VK_RIGHT: ev->tipo = TECLA_DERECHA; return OK;
            default:
                /* Unhandled key, ignore and continue */
                continue;
        }
    }
}

void terminalObtenerTam(
    tTerminal *term,
    unsigned *filas,
    unsigned *columnas
) {
    tWin32TermBackend *backend;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(!term || !term->backend || !filas || !columnas) {
        return;
    }

    backend = term->backend;

    if(GetConsoleScreenBufferInfo(backend->stdoutHandle, &csbi) == 0) {
        *filas = 0;
        *columnas = 0;
        return;
    }

    *filas = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *columnas = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void terminalActualizar(tTerminal *term)
{
    tWin32TermBackend *backend;
    DWORD written = 0;

    if(!term || !term->backend) {
        return;
    }

    backend = term->backend;

    /* If there is buffered output in term->buffer, write it to the console */
    if (term->buffer && term->len > 0) {
        BOOL ok = WriteConsoleA(
            backend->stdoutHandle,
            (LPCVOID)term->buffer,
            (DWORD)term->len,
            &written,
            NULL
        );

        /* If WriteConsoleA failed, fallback to fwrite */
        if (!ok || written == 0) {
            fwrite(term->buffer, 1, term->len, stdout);
            fflush(stdout);
        }

        term->len = 0;
    }

    /* Keep cursor hidden and ensure screen is flushed */
    fputs("\x1b[?25l", stdout);
    fputs("\x1b[H\x1b[2J", stdout);
    fflush(stdout);
}
