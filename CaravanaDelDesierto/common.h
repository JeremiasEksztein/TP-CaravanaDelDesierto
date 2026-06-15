#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

#define OK      0
#define ERR     1

#define TRUE    1
#define FALSE   0

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef int (*fnCmp)(const void *, const void *);
typedef void (*fnImprimir)(const void *);
typedef void (*fnAccion)(void *, void *);

void eprintf(const char *msg, ...);

#endif
