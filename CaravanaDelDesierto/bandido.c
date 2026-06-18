#include <stdlib.h>

#include "bandido.h"

/**
 * @file bandido.c
 * @brief Implementacion de las operaciones sobre la entidad tBandido.
 *
 * Este modulo contiene las funciones para crear, consultar, modificar
 * la posicion y mover bandidos en el tablero de "Caravana del Desierto".
 * Los bandidos son enemigos que se desplazan por el tablero y le quitan
 * una vida al jugador si coinciden en la misma casilla.
 */

/**
 * @brief Inicializa un bandido con valores por defecto.
 *
 * Asigna la posicion inicial 0, genera un identificador aleatorio
 * entre 0 y 99999, y establece la posicion anterior en 0.
 *
 * @param bandido Puntero al tBandido a inicializar.
 */
void crearBandido(tBandido *bandido)
{
	bandido->pos = 0;
	bandido->id = rand() % 100000;
	bandido->posAnterior = 0;
}

/**
 * @brief Obtiene el identificador unico del bandido.
 *
 * @param bandido Puntero constante al tBandido consultado.
 * @return El identificador numerico del bandido.
 */
int obtenerIdBandido(const tBandido *bandido)
{
	return bandido->id;
}

/**
 * @brief Obtiene la posicion actual del bandido en el tablero.
 *
 * @param bandido Puntero al tBandido consultado.
 * @return La posicion actual (indice de casilla).
 */
int obtenerPosBandido(tBandido *bandido)
{
	return bandido->pos;
}

/**
 * @brief Obtiene la posicion anterior del bandido en el tablero.
 *
 * Util para conocer de donde venia el bandido antes de su ultimo
 * movimiento.
 *
 * @param bandido Puntero al tBandido consultado.
 * @return La posicion anterior (indice de casilla).
 */
int obtenerPosAnteriorBandido(tBandido *bandido)
{
	return bandido->posAnterior;
}

void actualizarPosBandido(tBandido *bandido, void *pos)
{
	bandido->pos = *(int *)pos;
}

void destruirBandido(tBandido *bandido)
{
	(void)bandido;
	/* No-op: bandidos are stored in a heap-allocated array freed
	   as a whole by AdministrarJuego. */
}

/**
 * @brief Coloca el bandido en una posicion especifica del tablero.
 *
 * Asigna directamente la posicion sin registrar la anterior.
 * Para movimientos con registro de posicion anterior, usar moverBandido().
 *
 * @param b Puntero al tBandido a reubicar.
 * @param pos Nueva posicion (indice de casilla).
 */
void colocarBandidoEnPos(tBandido *b, const int pos)
{
	b->pos = pos;
}

/**
 * @brief Mueve el bandido en el tablero con aritmetica modular.
 *
 * Registra la posicion actual como posAnterior, luego aplica el
 * desplazamiento y ajusta el resultado con modulo n para mantener
 * la posicion dentro de los limites del tablero circular.
 * Tambien almacena la magnitud del movimiento realizado.
 *
 * @param b Puntero al tBandido a mover.
 * @param mov Cantidad de casillas a moverse (negativo = izquierda).
 * @param n Cantidad total de casillas del tablero (para el modulo).
 */
void moverBandido(tBandido *b, const int mov, const int n)
{
	b->posAnterior = b->pos;
	b->pos += mov;
	b->pos = ((b->pos % n) + n) % n;
	b->mov = mov;
}
int BuscarIndiceDeBandido(const tBandido* arrayBandidos, const int id, int cantBandidos)
{
    int i;
	for(i = 0; i < cantBandidos; i++)
	{
		if(arrayBandidos[i].id == id)
		{
			return i;
		}
	}
	return -1;
}
