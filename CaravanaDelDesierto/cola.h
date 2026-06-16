#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef struct sNodo {
    struct sNodo *sig;
    unsigned n;
    void *data;
};
typedef struct sNodo tNodo;

typedef struct {
    tNodo *pri, *ult;
} tCola;

void colaCrear(tCola *c);

int colaEncolar(tCola *c, const void *data, unsigned n);

int colaDesencolar(tCola *c, void *buf, unsigned n);

int colaFrente(const tCola *c, void *buf, unsigned n);

int colaEstaVacia(const tCola *c);

void colaDestruir(tCola *c);

#endif
