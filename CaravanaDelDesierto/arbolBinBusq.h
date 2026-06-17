#ifndef ARBOLBINBUSQ_H_INCLUDED
#define ARBOLBINBUSQ_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef enum {
    E_INORDEN,
    E_POSTORDEN,
    E_PREORDEN
} eTipoRecorrido;

typedef struct sNodoArbol tNodoArbol;
struct sNodoArbol {
    tNodoArbol *izq, *der;
    unsigned n;
    void *data;
};

typedef tNodoArbol *tArbolBinBusq;

void arbolBinBusqCrear(tArbolBinBusq *bst);

int arbolBinBusqPoner(
    tArbolBinBusq *bst,
    const void *data,
    unsigned n,
    fnCmp cmp
);

int arbolBinBusqSacar(
    tArbolBinBusq *bst,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
);

int arbolBinBusqBuscar(
    const tArbolBinBusq *bst,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
);

void arbolBinBusqImprimir(
    const tArbolBinBusq *bst,
    eTipoRecorrido tipo,
    fnImprimir imprimir
);

void arbolBinBusqVisitar(
    tArbolBinBusq *bst,
    eTipoRecorrido tipo,
    fnAccion accion,
    void *usuario
);

void arbolBinBusqDestruir(tArbolBinBusq *bst);

int arbolBinBusqContarNodos(const tArbolBinBusq *a);

#endif
