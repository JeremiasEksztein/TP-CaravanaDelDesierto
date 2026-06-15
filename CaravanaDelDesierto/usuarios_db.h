#ifndef USUARIOS_DB_H_INCLUDED
#define USUARIOS_DB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbolBinBusq.h"
#include "common.h"

#define JUGADORES_DB    "jugadores.dat"
#define PARTIDA_DB      "partidas.dat"

#define NOMBRECUENTALEN     64
#define NOMBREUSUARIOLEN    64

typedef struct {
    char nombreCuenta[NOMBRECUENTALEN];
    char nombreUsuario[NOMBREUSUARIOLEN];
    int borradoLogico; /* 0: activo, 1: eliminado */
} tRegistroJugador;

typedef struct {
    char nombreCuenta[NOMBRECUENTALEN]; /* FK(Jugadores) */
    int idPartida; /* PK */
    int cantJugadas;
    int puntaje;
    int borradoLogico; /* 0: activo, 1: eliminado */
} tRegistroPartida;

typedef struct {
    long offset;
    char nombreCuenta[NOMBRECUENTALEN];
} tIndiceJugadores;

typedef struct {
    long offset;
    char nombreCuenta[NOMBRECUENTALEN];
    int idPartida;
} tIndicePartidas;

typedef struct {
    tArbolBinBusq indice;
    const char *pathDatos;
    const char *pathIndice;    
} tTabla;

int tablaCrear(tTabla *t, const char *pathDatos, const char *pathIndice);



int tablaCerrar(tTabla *t);

void tablaDestruir(tTabla *t);

/*

int crearArchivoJugadores(const char *path);
int crearArchivoPartidas(const char *path);

int abrirArchivoJugadores(tArbolBinBusq *indice, const char *path);
int abrirArchivoPartidas(tArbolBinBusq *indice, const char *path);

int agregarJugador(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const tRegistroJugador *registro
);

int agregarPartida(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const tRegistroPartida *registro
);

int obtenerJugador(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const char *nombreCuenta, 
    tRegistroJugador *registro
);

int obtenerPartida(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const char *nombreCuenta, 
    int idPartida, 
    tRegistroPartida *registro
);

int eliminarJugador(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const char *nombreCuenta
);

int eliminarPartida(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const char *nombreCuenta, 
    int idPartida
);

int modificarJugador(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const tRegistroJugador *registro
);

int modificarPartida(
    tArbolBinBusq *indice, 
    FILE *fp, 
    const tRegistroPartida *registro
);

int cerrarArchivoJugadores(tArbolBinBusq *indice, FILE *fp);
int cerrarArchivoPartidas(tArbolBinBusq *indice, FILE *fp);

*/

#endif
