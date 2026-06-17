/**
 * @file jugador.c
 * @brief Implementación de las operaciones sobre el jugador del juego
 *        "Caravana del Desierto".
 *
 * Contiene las funciones que manipulan el estado del jugador:
 * inicialización, movimiento en el tablero, gestión de vidas,
 * puntos, invulnerabilidad y control de turno.
 */

#include <string.h>
#include <stdio.h>

#include "jugador.h"

/**
 * @brief Inicializa un jugador con nombre, vidas y valores por defecto.
 *
 * Establece el nombre y las vidas iniciales. Los puntos se inicializan
 * en cero, la invulnerabilidad se activa (el jugador comienza protegido),
 * el turno no se omite, y la posición se establece en la casilla de inicio.
 *
 * @param j Puntero al jugador a inicializar.
 * @param name Nombre del jugador (hasta TAM_NOMBRE caracteres).
 * @param vidas Cantidad de vidas iniciales del jugador.
 */
void crearJugador(tJugador *j, const char *name, const int vidas)
{
	strcpy(j->name, name);
	j->vidas = vidas;
	j->puntos = 0;
	j->invulnerable = 1;
	j->omitirTurno = 0;
	j->posAnterior = 0;
	j->pos = 0;
	j->mov = 0;
}

/**
 * @brief Obtiene la posición actual del jugador en el tablero.
 *
 * @param j Puntero constante al jugador.
 * @return Índice de la casilla donde se encuentra el jugador.
 */
int obtenerPosJugador(const tJugador *j)
{
	return j->pos;
}

/**
 * @brief Obtiene la posición anterior del jugador en el tablero.
 *
 * Útil para limpiar el rastro del jugador al moverlo o para
 * revertir un movimiento.
 *
 * @param j Puntero constante al jugador.
 * @return Índice de la casilla donde estaba el jugador antes del último movimiento.
 */
int obtenerPosAnteriorJugador(const tJugador *j)
{
	return j->posAnterior;
}

/**
 * @brief Decrementa en uno las vidas del jugador.
 *
 * @param j Puntero al jugador al que se le quita una vida.
 */
void quitarVida(tJugador *j)
{
	j->vidas--;
}

/**
 * @brief Decrementa en uno las vidas del jugador.
 *
 * @param j Puntero al jugador al que se le quita una vida.
 */
void obtenerVida(tJugador *j)
{
	j->vidas++;
}

/**
 * @brief Incrementa en uno los puntos del jugador.
 *
 * Se invoca cuando el jugador cae en una casilla de premio.
 *
 * @param j Puntero al jugador que recibe el punto.
 */
void obtenerPunto(tJugador *j)
{
	j->puntos++;
}

/**
 * @brief Activa el estado de invulnerabilidad del jugador.
 *
 * Mientras está invulnerable, los bandidos no le quitan vidas
 * aunque coincidan en la misma casilla.
 *
 * @param j Puntero al jugador a proteger.
 */
void hacerInvulnerable(tJugador *j)
{
	j->invulnerable = 1;
}

/**
 * @brief Desactiva el estado de invulnerabilidad del jugador.
 *
 * Tras moverse, el jugador deja de ser invulnerable y vuelve
 * a ser susceptible al ataque de los bandidos.
 *
 * @param j Puntero al jugador cuya invulnerabilidad se desactiva.
 */
void quitarInvulnerable(tJugador *j)
{
	j->invulnerable = 0;
}

/**
 * @brief Activa la bandera para omitir el próximo turno del jugador.
 *
 * Se invoca cuando el jugador cae en una casilla de tormenta.
 *
 * @param j Puntero al jugador que perderá el próximo turno.
 */
void omitirTurno(tJugador *j)
{
	j->omitirTurno = 1;
}

/**
 * @brief Desactiva la bandera de omisión de turno del jugador.
 *
 * Se invoca después de que el turno del jugador fue efectivamente omitido.
 *
 * @param j Puntero al jugador cuya omisión de turno se desactiva.
 */
void quitarOmitirTurno(tJugador *j)
{
	j->omitirTurno = 0;
}

/**
 * @brief Coloca al jugador en una posición específica del tablero.
 *
 * Guarda la posición actual como la posición anterior antes de
 * asignar la nueva.
 *
 * @param j Puntero al jugador a reubicar.
 * @param pos Nueva posición del jugador (índice de casilla).
 */
void colocarJugadorEnPos(tJugador *j, const int pos)
{
	j->posAnterior = j->pos;
	j->pos = pos;
}

/**
 * @brief Mueve al jugador a lo largo del tablero con efecto espejo en los bordes.
 *
 * Calcula la nueva posición del jugador aplicando un desplazamiento
 * (mov * dir) sobre la posición actual. El movimiento rebota en los
 * extremos del tablero: si la posición calculada excede el rango
 * [0, n-1], se refleja como en un espejo en lugar de salirse.
 *
 * Además, registra la magnitud del movimiento y desactiva la
 * invulnerabilidad, ya que moverse implica exponerse a los bandidos.
 *
 * @param j Puntero al jugador a mover.
 * @param mov Cantidad de casillas a desplazarse.
 * @param dir Dirección del movimiento (1 = derecha, -1 = izquierda).
 * @param n Cantidad total de casillas del tablero.
 */
void moverJugador(tJugador *j, const int mov, const int n)
{
	int range = n;

	j->posAnterior = j->pos;
	j->pos = j->pos + mov;
	j->pos = ((j->pos % n) + n) % n;
	printf("[DEBUG moverJugador] IN: pos=%d posAnt=%d mov=%d n=%d range=%d\n",
	       j->pos, j->posAnterior, mov, n, range);
	j->mov = mov;
	j->invulnerable = 0;
}
int ConsultarOmisionDeTurno(tJugador* t)
{
	return t->omitirTurno == OMISION ? OMISION : 0;
}
int ConsultarVidasJugador(tJugador* jugador)
{
	return jugador->vidas;
}
