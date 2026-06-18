/**
 * @file turnos.h
 * @brief Declaración de funciones para la creación de turnos de jugador y
 * bandido.
 */

#ifndef TURNOS_H
#define TURNOS_H
#include "bandido.h"
#include "jugador.h"
#include "tablero.h"

/**
 * @enum tTipoTurno
 * @brief Identifica el tipo de evento o turno: jugador o bandido.
 */
typedef enum { EVT_JUGADOR, EVT_BANDIDO } tTipoTurno;

/**
 * @struct tTurno
 * @brief Representa un turno individual del juego, ya sea de jugador o bandido.
 *
 * Almacena el tipo de turno, el identificador del participante,
 * la cantidad de pasos y la dirección del movimiento.
 */
typedef struct {
  tTipoTurno tipo; /**< Tipo de turno (EVT_JUGADOR o EVT_BANDIDO). */
  int id;  /**< 0 para jugador, o identificador del bandido. */
  int mov; /**< Cantidad de pasos a mover (1..6). */
  int dir; /**< Dirección del movimiento: +1 adelante, -1 atrás. */
} tTurno;

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
void crearTurnoJugador(tTurno *t, const int mov, const tTablero *tab,
                       const tJugador *j);

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



/**
 * @brief Inicia el proceso de turno del jugador.
 *
 * Resuelve el movimiento del jugador dentro de su turno, incluyendo
 * la tirada del dado y el desplazamiento sobre el tablero.
 *
 * @param turno Puntero a la estructura de turno a procesar.
 */
void IniciarElTurnoDelJugador(tTurno* turno);

/**
 * @brief Inicia el proceso de turno de un bandido específico.
 *
 * Resuelve el movimiento del bandido identificado, determinando
 * la dirección y cantidad de pasos hacia el jugador.
 *
 * @param turno Puntero a la estructura de turno a procesar.
 * @param idDelBandido Identificador del bandido que realiza el turno.
 */
void IniciarElTurnoDelBandido(tTurno* turno, int  idDelBandido);

/**
 * @brief Desordena aleatoriamente el vector de turnos.
 *
 * Aplica un algoritmo de mezcla (shuffle) para redistribuir el orden
 * de los turnos, garantizando así que la secuencia de jugadas sea
 * impredecible en cada ronda.
 *
 * @param turnos Puntero al vector de turnos a desordenar.
 * @param cantTotal Cantidad total de turnos en el vector.
 */
void DesordenarVectorDeTurnos(tTurno* turnos, int cantTotal);

/**
 * @brief Verifica si un turno corresponde al jugador.
 *
 * @param turno Puntero al turno a verificar.
 * @return 1 si el turno es de tipo EVT_JUGADOR, 0 en caso contrario.
 */
int esTurnoDeJugador(tTurno* turno);
                       #endif
