#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef struct sNodo2 tNodo2;
struct sNodo2 {
  tNodo2 *ant, *sig;
  unsigned n;
  void *data;
};

typedef tNodo2 *tListaCircularDoble;

void listaCircularDobleCrear(tListaCircularDoble *l);

int listaCircularDobleEmpujar(tListaCircularDoble *l, const void *data,
                              unsigned n);

int listaCircularDobleBuscar(tListaCircularDoble *l, const void *clave,
                             void *buf, unsigned n, fnCmp cmp);

int listaCircularDobleActualizarEnPos(tListaCircularDoble *l, const void *data,
                                      int pos, fnAccion accion);

unsigned listaCircularDobleLargo(const tListaCircularDoble *l);

void listaCircularDobleDestruir(tListaCircularDoble *l);
int listaCircularDobleBuscarPos(tListaCircularDoble *l, const void *d,
                                fnCmp cmp);

#endif
