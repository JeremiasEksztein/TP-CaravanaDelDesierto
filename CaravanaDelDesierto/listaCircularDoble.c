#include "listaCircularDoble.h"

void listaCircularDobleCrear(tListaCircularDoble *l)
{
    if(!l) {
        return;
    }

    *l = NULL;
}

int listaCircularDobleEmpujar(
    tListaCircularDoble *l,
    const void *data,
    unsigned n
) {
    tNodo2 nuevo;

    if(!l || !data) {
        return ERR;
    }

    nuevo = malloc(sizeof(tNodo2));

    if(!nuevo) {
        return ERR;
    }

    nuevo->data = malloc(n);

    if(!nuevo->data) {
        free(nuevo);
        return ERR;
    }

    memcpy(nuevo->data, data, n);
    nuevo->n = n;

    if(!*l) {
        (*l)->sig = nuevo;
        (*l)->ant = nuevo;
        (*l) = nuevo;
    } else {
        nuevo->ant = (*l)->ant;
        nuevo->sig = (*l);
        nuevo->ant->sig = nuevo;
        (*l)->sig = nuevo;
    }

    return OK;
}

int listaCircularDobleBuscar(
    tListaCircularDoble *l,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
) {
    int compar;
    tNodo2 *comienzo = *l;

    if(!l || !clave || !buf || !cmp) {
        return ERR;
    }

    while(*l != comienzo) {
        compar = cmp((*l)->data, clave);

        if(compar == 0) {
            memcpy(buf, (*l)->data, MIN((*l)->n, n));
            return OK;
        }

        l = &(*l)->sig;
    }

    return ERR;
}

void listaCircularDobleDestruir(tListaCircularDoble *l)
{
    tNodo2 *tmp;

    while(*l) {
        tmp = *l;
        l = &(*l)->sig;

        tmp->ant->sig = tmp->sig;
        tmp->sig->ant = tmp->ant;

        free(tmp->data);
        free(tmp);
        tmp = NULL;
    }
}
