#include "terminal.h"

#include <Windows.h>

struct sTerminal {
    HANDLE stdinHandle;
    HANDLE stdoutHandle;
    DWORD modoOriginal;
};

int terminalCrear(tTerminal **term)
{
    if(!term) {
        return ERR;
    }

    *term = malloc(sizeof(tTerminal));

    if(!*term) {
        return ERR;
    }
}

void terminalDestruir(tTerminal *term);

int terminalEntrarModoCrudo(tTerminal *term);
void terminalSalirModoCrudo(tTerminal *term);

void terminalLimpiar(tTerminal *term);

void terminalMoverCursor(
    tTerminal *term,
    unsigned fila,
    unsigned columna
);

void terminalOcultarCursor(tTerminal *term);
void terminalMostrarCursor(tTerminal *term);

void terminalColorTexto(
    tTerminal *term,
    eColor color
);

void terminalColorFondo(
    tTerminal *term,
    eColor color
);

void terminalRestablecerAtributos(tTerminal *term);

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
