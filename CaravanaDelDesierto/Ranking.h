#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#if defined(__linux__) || defined(__gnu_linux__)
    #define _XOPEN_SOURCE 501
#endif

#include "jugador.h"
#include "usuarios_db.h"
#include "arbolBinBusq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char nombre[11];
	int puntosAcumulados;
	int movimientosAcumulados;
} tNodoRanking;

int MostrarRanking(tTabla *tablaJugadores);

#endif
