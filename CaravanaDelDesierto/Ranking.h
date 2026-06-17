#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

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
