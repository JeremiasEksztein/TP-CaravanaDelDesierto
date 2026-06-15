#ifndef USUARIOS_DB_H_INCLUDED
#define USUARIOS_DB_H_INCLUDED

#include "arbolBinBusq.h"
#include "common.h"

#define JUGADORES_DB    "jugadores.dat"
#define PARTIDA_DB      "partidas.dat"

#define NOMBRECUENTALEN     64
#define NOMBREUSUARIOLEN    64

typedef struct {
    char nombreCuenta[NOMBRECUENTALEN];
    char nombreUsuario[NOMBREUSUARIOLEN];
} tRegistroJugador;

typedef struct {
    char nombreCuenta[NOMBRECUENTALEN];
    int idPartida;
    int cantJugadas;
    int puntaje;
} tRegistroPartida;

int crearArchivoJugadores(const char *path);
int crearArchivoPartidas(const char *path);



#endif
