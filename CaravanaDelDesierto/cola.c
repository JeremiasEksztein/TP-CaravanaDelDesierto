#include "cola.h"

void colaCrear(tCola *c)
{
    if(!c) {
        return;
    }

    c->pri = c->ult = NULL;
}

int colaEncolar(tCola *c, const void *data, unsigned n)
{
    tNodo *nuevo;

    if(!c || !data) {
        return ERR;
    }

    nuevo = malloc(sizeof(tNodo));

    if(!nuevo) {
        return ERR;
    }

    nuevo->data = malloc(n);

    if(!nuevo->data) {
        free(nuevo);
        return ERR;
    }

    if(!c->pri) {
        c->pri = nuevo;
    } else {
        c->ult->sig = nuevo;
    }

    q->ult = nuevo;

    return OK;
}

int colaDesencolar(tCola *c, void *buf, unsigned n)
{
    tNodo borrar;

    if(!c || !c->pri || !c->ult || !buf) {
        return ERR;
    }

    borrar = c->pri;
    c->pri = c->pri->sig;

    memcpy(buf, borrar->data, MIN(borrar->n, n));

    free(borrar->data);
    free(borrar);

    return OK;
}

int colaFrente(const tCola *c, void *buf, unsigned n)
{
    tNodo tmp;

    if(!c || !c->pri || !c->ult || !buf) {
        return ERR;
    }

    memcpy(buf, c->pri->data, MIN(c->pri->n, n));

    return OK;
}

int colaEstaVacia(const tCola *c)
{
    if(!c) {
        return TRUE;
    }

    return (c->pri == c->ult) ? TRUE : FALSE;
}

void colaDestruir(tCola *c)
{
    tNodo *tmp;

    while(c->pri) {
        tmp = c->pri;
        c->pri = c->pri->sig;
        free(tmp->data);
        free(tmp);
    }

    c->pri = c->ult = NULL;
}
