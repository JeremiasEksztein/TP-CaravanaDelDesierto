#ifndef ADMINISTRADORDEJUEGO_H_INCLUDED
#define ADMINISTRADORDEJUEGO_H_INCLUDED

#include "juego.h"
#include "Ranking.h"
#include "cola.h"
#include "interfaz.h"
#include "usuarios_db.h"

#define AGREGADO 2
#define MOSTRAR 1
#define JUGADOR_GANO 0
#define MEMORIA_LLENA -1
#define DERROTA -1
#define JUEGO_CONTINUA 1

int AdministrarJuego(const char *nombreJugador, int idJugador,
                     tTabla *tablaJugadores, tRegistroPartida *partidaOut);

int AdministrarRanking(int operacion, tTabla *tablaJugadores);

int Jugar(tJuego* jue, tJugador* jug, tTablero* partida,
          int idJugador, tRegistroPartida *partidaOut);

#endif
