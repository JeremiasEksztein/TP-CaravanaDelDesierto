#include "terminal.h"

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

typedef struct {
    struct termios modoOriginal;
} tUnixTermBackend;

int terminalCrear(tTerminal *term)
{
    tUnixTermBackend *backend;

    if(!term) {
        return 1;
    }

    backend = malloc(sizeof(tUnixTermBackend));

    if(!backend) {
        return 1;
    }

    term->backend = backend;

    return 0;
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
    tUnixTermBackend *backend;
    struct termios modoCrudo;

    if(!term || !term->backend) {
        return 1;
    }

    backend = term->backend;

    if(tcgetattr(STDIN_FILENO, &backend->modoOriginal) == -1) {
        return 1;
    }

    modoCrudo = backend->modoOriginal;
    modoCrudo.c_iflag &= (unsigned)~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    modoCrudo.c_oflag &= (unsigned)~(OPOST);
    modoCrudo.c_cflag |= (CS8);
    modoCrudo.c_lflag &= (unsigned)~(ECHO | ICANON | IEXTEN | ISIG);
    modoCrudo.c_cc[VMIN] = 0;
    modoCrudo.c_cc[VTIME] = 0;

    if(tcsetattr(STDIN_FILENO, TCSANOW, &modoCrudo) == -1) {
        return 1;
    }

    return 0;
}

void terminalSalirModoCrudo(tTerminal *term)
{
    tUnixTermBackend *backend;

    if(!term || !term->backend) {
        return;
    }

    backend = term->backend;

    if(!tcsetattr(STDIN_FILENO, TCSANOW, &backend->modoOriginal)) {
        return;
    }
}

void terminalLimpiar(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }

    fputs("\x1b[H\x1b[2J", stdout);
    fflush(stdout);
}

void terminalMoverCursor(
    tTerminal *term,
    unsigned fila,
    unsigned columna
) {
    char buffer[32];

    if(!term || !term->backend) {
        return;
    }

    snprintf(buffer, sizeof(buffer), "\x1b[%u;%uH", fila + 1, columna + 1);
    fputs(buffer, stdout);
    fflush(stdout);
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
}

void terminalColorTexto(
    tTerminal *term,
    eColor color
) {
    const char *codigoColor;

      if(!term || !term->backend) {
        return;
    }

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
}

void terminalColorFondo(
    tTerminal *term,
    eColor color
) {
    const char *codigoColor;
    
    if(!term || !term->backend) {
        return;
    }

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
    fflush(stdout);
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
) {
    if(!term || !term->backend || !filas || !columnas) {
        return;
    }
}

void terminalActualizar(tTerminal *term)
{
    if(!term || !term->backend) {
        return;
    }


}
