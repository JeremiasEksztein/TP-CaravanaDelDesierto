#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>

typedef struct sTerminal tTerminal;

typedef enum {
    COLOR_NEGRO,
    COLOR_ROJO,
    COLOR_VERDE,
    COLOR_AMARILLO,
    COLOR_AZUL,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_BLANCO,
    COLOR_DEFAULT
} eColor;

typedef enum {
    TECLA_NINGUNA,
    TECLA_CARACTER,
    TECLA_ENTER,
    TECLA_ESC,
    TECLA_BACKSPACE,
    TECLA_TAB,
    TECLA_ARRIBA,
    TECLA_ABAJO,
    TECLA_IZQUIERDA,
    TECLA_DERECHA
} eTipoTecla;

typedef struct {
    eTipoTecla tipo;
    char caracter;
} tEventoTecla;

int terminalCrear(tTerminal **term);
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

#endif