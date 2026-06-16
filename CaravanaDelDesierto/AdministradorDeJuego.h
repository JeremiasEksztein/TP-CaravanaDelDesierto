#ifndef ADMINISTRADORDEJUEGO_H_INCLUDED
#define ADMINISTRADORDEJUEGO_H_INCLUDED

#include "juego.h"
#include "Ranking.h"
#include "cola.h"
#define AGREGADO 2
#define MOSTRAR 1
#define JUGADOR_GANO 0
#define MEMORIA_LLENA -1

int AdministrarJuego();

int AdministrarRanking(int operacion, void* extras);

int Jugar(tJuego* jue,tJugador* jug, tTablero* partida);
#endif