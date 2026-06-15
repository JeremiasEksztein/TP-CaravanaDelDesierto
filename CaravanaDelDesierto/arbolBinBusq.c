#include "arbolBinBusq.h"

void _inorden(const tArbolBinBusq *bst, fnImprimir imprimir);
void _preorden(const tArbolBinBusq *bst, fnImprimir imprimir);
void _postorden(const tArbolBinBusq *bst, fnImprimir imprimir);

void _mut_inorden(tArbolBinBusq *bst, fnAccion, void *usuario);
void _mut_preorden(tArbolBinBusq *bst, fnAccion, void *usuario);
void _mut_postorden(tArbolBinBusq *bst, fnAccion, void *usuario);

unsigned _alturaArbol(const tArbolBinBusq *bst)
{
    unsigned altL, altR;

    if(!bst || !*bst) {
        return 0;
    }

    altL = _alturaArbol(&(*bst)->izq);
    altR = _alturaArbol(&(*bst)->der);

    return MAX(altL, altR) + 1;
}

void _eliminarRaiz(tArbolBinBusq *bst)
{
    tNodoArbol *tmp;
    tArbolBinBusq *reemplazo;
    unsigned altL, altR;

    altL = _alturaArbol(&(*bst)->izq);
    altR = _alturaArbol(&(*bst)->der);

    if(altL > altR) {
        bst = &(*bst)->izq;

        while((*bst)->der) {
            bst = &(*bst)->der;
        }
    } else {
        bst = &(*bst)->der;

        while((*bst)->izq) {
            bst = &(*bst)->izq;
        }
    }

    (*reemplazo)->data = (*bst)->data;
    (*reemplazo)->n = (*bst)->n;
    tmp = (*bst);
    (*bst) = ((*bst)->izq) ? (*bst)->izq : (*bst)->der;

    free(tmp);
}

void arbolBinBusqCrear(tArbolBinBusq *bst)
{
    if(!bst) {
        return;
    }

    *bst = NULL;
}

int arbolBinBusqPoner(
    tArbolBinBusq *bst,
    const void *data,
    unsigned n,
    fnCmp cmp
) {
    int compar;

    if(!bst || !data || !cmp) {
        return ERR;
    }

    while(!*bst) {
        compar = cmp((*bst)->data, data);

        if(compar > 0) {
            bst = &(*bst)->izq;
        } else if(compar < 0) {
            bst = &(*bst)->der;
        } else {
            return ERR;
        }
    }

    *bst = malloc(sizeof(tNodoArbol));

    if(!*bst) {
        return ERR;
    }

    (*bst)->data = malloc(n);

    if(!(*bst)->data) {
        free(*bst);
        *bst = NULL;
        return ERR;
    }

    memcpy((*bst)->data, data, n);
    (*bst)->n = n;
    (*bst)->izq = (*bst)->der = NULL;

    return OK;
}

int arbolBinBusqSacar(
    tArbolBinBusq *bst,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
) {
    int compar;

    if(!bst || !clave || !buf || !cmp) {
        return ERR;
    }

    while(!*bst) {
        compar = cmp((*bst)->data, data);

        if(compar > 0) {
            bst = &(*bst)->izq;
        } else if(compar < 0) {
            bst = &(*bst)->der;
        } else {
            memcpy(buf, (*bst)->data, MIN((*bst)->n, n));

            free((*bst)->data);

            if(!(*bst)->izq && !(*bst)->der) {
                free(*bst);
                *bst = NULL;
            } else {
                _eliminarRaiz(bst);
            }

            return OK;
        }
    }

    return ERR;
}

int arbolBinBusqBuscar(
    const tArbolBinBusq *bst,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
) {
    int compar;

    if(!bst || !clave || !buf || !cmp) {
        return ERR;
    }

    while(!*bst) {
        compar = cmp((*bst)->data, data);

        if(compar > 0) {
            bst = &(*bst)->izq;
        } else if(compar < 0) {
            bst = &(*bst)->der;
        } else {
            memcpy(buf, (*bst)->data, MIN((*bst)->n, n));

            return OK;
        }
    }

    return ERR;
}

void arbolBinBusqImprimir(
    const tArbolBinBusq *bst,
    eTipoRecorrido tipo,
    fnImprimir imprimir
) {
    if(!bst || !imprimir) {
        return;
    }

    switch(tipo) {
        case E_INORDEN:
            _inorden()
        case E_PREORDEN:
        case E_POSTORDEN:
        default:
            return;
    }
}

void arbolBinBusqVisitar(
    tArbolBinBusq *bst,
    eTipoRecorrido tipo,
    fnAccion accion,
    void *usuario
);

void arbolBinBusqDestruir(tArbolBinBusq *bst)
{
    if(!bst || !*bst) {
        return;
    }

    arbolBinBusqDestruir(&(*bst)->izq);
    arbolBinBusqDestruir(&(*bst)->der);

    free((*bst)->data);
    free(*bst);

    *bst = NULL;
}
