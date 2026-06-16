#ifndef CANVAS_H
#define CANVAS_H

#include "terminal.h"

typedef struct {
    char ch;
    eColor fg;
    eColor bg;
} tCelda;

typedef struct {
    unsigned ancho;
    unsigned alto;
    tCelda *celdas;
} tCanvas;

int canvasCrear(
    tCanvas *canvas,
    unsigned ancho,
    unsigned alto
);

void canvasDestruir(tCanvas *canvas);

void canvasLimpiar(
    tCanvas *canvas,
    char ch,
    eColor fg,
    eColor bg
);

int canvasPonerCelda(
    tCanvas *canvas,
    int x,
    int y,
    const tCelda *celda
);

int canvasObtenerCelda(
    const tCanvas *canvas,
    int x,
    int y,
    tCelda *celda
);

unsigned canvasAncho(
    const tCanvas *canvas
);

unsigned canvasAlto(
    const tCanvas *canvas
);

void canvasRenderizar(
    const tCanvas *canvas,
    tTerminal *term
);

#endif