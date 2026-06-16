#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "jugador.h"
#include "usuarios_db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NOMBRE_ARCHIVO_RANKING "Ranking.bin"

int MostrarRankingDeJugadores();
int GuardarJugadorEnRanking(tJugador* jugador);

#endif