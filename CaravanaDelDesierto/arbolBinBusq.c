#include "arbolBinBusq.h"

void arbolBinBusqCrear(tArbolBinBusq *bst)
{


}

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
