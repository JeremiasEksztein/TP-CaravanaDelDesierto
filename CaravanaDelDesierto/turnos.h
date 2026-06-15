/**
 * @file turnos.h
 * @brief Declaración de funciones para la creación de turnos de jugador y
 * bandido.
 */

#ifndef TURNOS_H
#define TURNOS_H

#include "tipos.h"

/**
 * @brief Crea un turno para un jugador en el tablero.
 *
 * Calcula la nueva posición del jugador en función del movimiento
 * (con signo: positivo avanza, negativo retrocede). Si el movimiento
 * excede los límites del tablero, la posición se ajusta con rebote.
 *
 * @param t Puntero a la estructura tTurno donde se almacenará el turno creado.
 * @param mov Cantidad de casilleros a moverse (con signo).
 * @param tab Puntero al tablero actual.
 * @param j Puntero al jugador que realiza el turno.
 */
void crearTurnoJugador(tTurno *t, const int mov,
                       const tTablero *tab, const tJugador *j);

/**
 * @brief Crea un turno para un bandido que persigue al jugador.
 *
 * Determina la dirección óptima hacia el jugador calculando la distancia más
 * corta (hacia adelante o hacia atrás) sobre el tablero circular y asigna el
 * movimiento correspondiente.
 *
 * @param t Puntero a la estructura tTurno donde se almacenará el turno creado.
 * @param b Puntero al bandido que realiza el turno.
 * @param j Puntero al jugador al que persigue el bandido.
 * @param tab Puntero al tablero actual.
 * @param mov Cantidad de casilleros que se mueve el bandido.
 */
void crearTurnoBandido(tTurno *t, const tBandido *b, const tJugador *j,
                       const tTablero *tab, const int mov);
#endif
