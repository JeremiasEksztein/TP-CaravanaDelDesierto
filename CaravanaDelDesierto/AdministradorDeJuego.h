#ifndef ADMINISTRADORDEJUEGO_H_INCLUDED
#define ADMINISTRADORDEJUEGO_H_INCLUDED

#include "juego.h"
#include "Ranking.h"

#define AGREGADO 2
#define MOSTRAR 1
#define JUGADOR_GANO 0

int AdministrarInicioJuego();

int AdministrarRanking(int operacion, void* extras);

#endif