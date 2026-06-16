/**
 * @file turnos.c
 * @brief Implementación de las funciones para la creación de turnos de jugador
 * y bandido.
 */

#include "turnos.h"
#include <stdio.h>

/**
 * @brief Crea un turno para un jugador en el tablero.
 *
 * Calcula la nueva posición del jugador a partir de su posición actual y el
 * movimiento con signo. Si la suma de la posición actual y el movimiento
 * supera los límites del tablero, se aplica un rebote: el desplazamiento se
 * recalcula para reflejar el rebote en el borde.
 *
 * @param t Puntero a la estructura tTurno donde se almacenará el turno creado.
 * @param mov Cantidad de casilleros a moverse (con signo).
 * @param tab Puntero al tablero actual.
 * @param j Puntero al jugador que realiza el turno.
 */
void crearTurnoJugador(tTurno *t, const int mov, const tTablero *tab,
		       const tJugador *j)
{
	int jp = j->pos, casillaFinal = tab->cant - 1;
	int movAbs = mov;
	int dir = 1;

	if (mov < 0) {
		movAbs = -mov;
		dir = -1;
	}

	t->tipo = EVT_JUGADOR;
	t->id = 0;
	if (jp + movAbs > casillaFinal && dir == 1) {
		t->mov = (-2 * jp - movAbs + 2 * casillaFinal);
		printf("Posicion Actual: %d; Movimiento Jugador: %d, Nueva posicion: %d\n",
		       jp, t->mov, t->mov);
		return;
	}
	if (jp - movAbs < 0 && dir == -1) {
		t->mov = (2 * jp - movAbs);
		printf("Posicion Actual: %d; Movimiento Jugador: %d, Nueva posicion: %d\n",
		       jp, t->mov, t->mov);
		if (t->mov < 0) {
			t->mov = -t->mov;
		}
		return;
	}
	t->mov = mov;
}

/**
 * @brief Crea un turno para un bandido que persigue al jugador.
 *
 * Determina la dirección óptima de movimiento calculando las distancias hacia
 * adelante y hacia atrás sobre el tablero circular mediante aritmética modular.
 * El bandido elige la dirección que implique la menor distancia hacia el
 * jugador.
 *
 * @param t Puntero a la estructura tTurno donde se almacenará el turno creado.
 * @param b Puntero al bandido que realiza el turno.
 * @param j Puntero al jugador al que persigue el bandido.
 * @param tab Puntero al tablero actual.
 * @param mov Cantidad de casilleros que se mueve el bandido.
 */
void crearTurnoBandido(tTurno *t, const tBandido *b, const tJugador *j,
		       const tTablero *tab, const int mov)
{
	int n = tab->cant;
	int bp = b->pos;
	int jp = j->pos;

	int distF = (jp - bp + n) % n;
	int distB = (bp - jp + n) % n;

	t->tipo = EVT_BANDIDO;
	t->id = b->id;

	if (distF <= distB) {
		t->mov = mov;
	} else {
		t->mov = -mov;
	}
}
void IniciarElTurnoDelJugador(tTurno* turno)
{
	turno->tipo = EVT_JUGADOR;
	turno->id = 0; //Id del jugador;
	turno->mov = 0;
	turno->dir = 0;
}
void IniciarElTurnoDelBandido(tTurno* turno, int  idDelBandido)
{
	turno->tipo = EVT_BANDIDO;
	turno->id = idDelBandido;
	turno->mov = 0;
	turno->dir = 0;
}
void DesordenarVectorDeTurnos(tTurno* turnos, int cantTotal)
{
	int nroTurno;
	int i;
	tTurno auxSwapping;
	//permutar artificialmente
	for(i = 0; i < cantTotal ; i++)
	{
		nroTurno = rand()%(cantTotal-i);
		auxSwapping = *(turnos+i);
		*(turnos+i) = *(turnos+nroTurno);
		*(turnos+nroTurno) = auxSwapping;
	}
}
int esTurnoDeJugador(tTurno* turno)
{
	return turno->tipo == EVT_JUGADOR ? 1 : 0;
}