#ifndef USUARIOS_DB_H_INCLUDED
#define USUARIOS_DB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbolBinBusq.h"
#include "common.h"

#define JUGADORES_DB    "jugadores.dat"
#define PARTIDA_DB      "partidas.dat"

#define ARCHPATHLEN         256

#define INDICE_CLAVE_LEN    256

#define NOMBRECUENTALEN     64
#define NOMBREUSUARIOLEN    64

typedef int (*fnLeerClave)(void *, const void *);

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
    FILE *archDatos;

    tArbolBinBusq indice;
    const char pathDatos[ARCHPATHLEN];
    const char pathIndice[ARCHPATHLEN];    
    unsigned regTam, claveTam;

    fnCmp cmp;
    fnLeerClave leerClave;
    
    int indiceSucio;
} tTabla;


typedef struct { 
    char clave[INDICE_CLAVE_LEN];
    long offset;
} tEntradaIndice;

int tablaCrear(
    tTabla *t, 
    const char *pathDatos, 
    const char *pathIndice, 
    fnCmp cmp,
    fnLeerClave leer,
    unsigned regTam, 
    unsigned claveTam
);

int jugadorIndiceCmp(const void *a, const void *b);
int jugadorLeerClave(void *out, void *in);

int partidaIndiceCmp(const void *a, const void *b);
int partidaLeerClave(void *out, void *in);

int tablaAbrir(tTabla *t);

int tablaIngresar(tTabla *t, const void *reg);

int tablaBuscar(tTabla *t, const void *clave, void *buf);

int tablaActualizar(tTabla *t, const void *clave, const void *reg);

int tablaRecorrer(tTabla *t, fnAccion accion, void *usuario);

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
