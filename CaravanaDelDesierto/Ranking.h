/**
 * @file Ranking.h
 * @brief Funciones para la generacion y visualizacion del ranking de jugadores.
 *
 *  Este modulo define la estructura que representa una entrada del ranking
 *  y la funcion encargada de calcular y mostrar la tabla de posiciones
 *  ordenada por puntos acumulados.
 */

#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "jugador.h"
#include "usuarios_db.h"
#include "arbolBinBusq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct tNodoRanking
 * @brief Entrada del ranking con los datos acumulados de un jugador.
 *
 *  Cada nodo almacena el identificador, nombre, puntos y cantidad de
 *  movimientos totales de un jugador a lo largo de todas sus partidas.
 */
typedef struct {
	/** @brief Identificador unico del jugador. */
	int id;
	/** @brief Nombre del jugador (maximo 10 caracteres + nulo). */
	char nombre[11];
	/** @brief Suma total de puntos obtenidos en todas las partidas. */
	int puntosAcumulados;
	/** @brief Cantidad total de movimientos realizados en todas las partidas. */
	int movimientosAcumulados;
} tNodoRanking;

/**
 * @brief Calcula y muestra el ranking de jugadores por pantalla.
 *
 *  Lee las partidas registradas, acumula puntos y movimientos por jugador,
 *  resuelve los nombres desde el indice de la tabla y muestra la tabla
 *  ordenada de mayor a menor puntaje.
 *
 * @param tablaJugadores Puntero a la tabla indexada de jugadores para
 *                       resolver nombres a partir de IDs.
 * @return OK (0) si la operacion se completo correctamente, ERR si fallo.
 */
int MostrarRanking(tTabla *tablaJugadores);

#endif
