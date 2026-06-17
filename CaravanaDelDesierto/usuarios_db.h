#ifndef USUARIOS_DB_H_INCLUDED
#define USUARIOS_DB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbolBinBusq.h"
#include "common.h"

#define TAM_NOMBRE_JUG 11

#define JUGADORES_DB "jugadores.dat"
#define JUGADORES_IDX "jugadores.idx"
#define PARTIDA_DB "partidas.dat"

#define ARCHPATHLEN 256

#define INDICE_CLAVE_LEN 256

typedef int (*fnLeerClave)(void *, const void *);

typedef struct {
  int id;
  char nombre[TAM_NOMBRE_JUG];
  int partidasJugadas;
} tRegistroJugador;

typedef struct {
  int idJugador;
  int puntosObtenidos;
  int movimientosRealizados;
} tRegistroPartida;

typedef struct {
  FILE *archDatos;

  tArbolBinBusq indice;
  char pathDatos[ARCHPATHLEN];
  char pathIndice[ARCHPATHLEN];
  unsigned regTam, claveTam;

  fnCmp cmp;
  fnLeerClave leerClave;

  int indiceSucio;
} tTabla;

typedef struct {
  char clave[INDICE_CLAVE_LEN];
  long offset;
  int id;
} tEntradaIndice;

int tablaCrear(tTabla *t, const char *pathDatos, const char *pathIndice,
               fnCmp cmp, fnLeerClave leer, unsigned regTam, unsigned claveTam);

int jugadorIndiceCmp(const void *a, const void *b);
int jugadorLeerClave(void *out, const void *in);

int tablaAbrir(tTabla *t);

int tablaIngresar(tTabla *t, const void *reg);

int tablaBuscar(tTabla *t, const void *clave, void *buf);

int tablaActualizar(tTabla *t, const void *clave, const void *reg);

int tablaRecorrer(tTabla *t, fnAccion accion, void *usuario);

int tablaCerrar(tTabla *t);

void tablaDestruir(tTabla *t);

int tablaProximoId(tTabla *t);


#endif
