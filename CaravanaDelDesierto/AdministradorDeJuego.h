/**
 * @file AdministradorDeJuego.h
 * @brief Orquestador principal del ciclo de vida de una partida y del ranking.
 *
 *  Este modulo agrupa las funciones de alto nivel que coordinan la
 *  inicializacion de una partida, el bucle de turnos, la persistencia
 *  de resultados y la administracion del ranking de jugadores.
 */

#ifndef ADMINISTRADORDEJUEGO_H_INCLUDED
#define ADMINISTRADORDEJUEGO_H_INCLUDED

#include "juego.h"
#include "Ranking.h"
#include "cola.h"
#include "interfaz.h"
#include "usuarios_db.h"

/** @def AGREGADO
 *  @brief Codigo de retorno: se agrego un jugador nuevo al sistema. */
#define AGREGADO 2

/** @def MOSTRAR
 *  @brief Codigo de retorno: se solicito mostrar informacion (ranking). */
#define MOSTRAR 1

/** @def JUGADOR_GANO
 *  @brief Codigo de retorno: el jugador llego a la meta y gano la partida. */
#define JUGADOR_GANO 0

/** @def MEMORIA_LLENA
 *  @brief Codigo de retorno: no se pudo asignar memoria dinamica. */
#define MEMORIA_LLENA -2

/** @def DERROTA
 *  @brief Codigo de retorno: el jugador perdio todas sus vidas. */
#define DERROTA -1

/** @def JUEGO_CONTINUA
 *  @brief Codigo de retorno: la partida aun no ha terminado. */
#define JUEGO_CONTINUA 1

/**
 * @brief Administra el ciclo completo de una partida de un jugador.
 *
 *  Carga la configuracion, inicializa el jugador, ejecuta el bucle de
 *  turnos y, al finalizar, persiste los resultados en el archivo de
 *  partidas y actualiza el contador de partidas jugadas en la tabla
 *  de jugadores.
 *
 * @param nombreJugador   Nombre del jugador que inicia la partida.
 * @param idJugador       Identificador unico del jugador en la tabla.
 * @param tablaJugadores  Puntero a la tabla indexada de jugadores;
 *                         puede ser NULL si no se desea persistencia.
 * @param partidaOut      Puntero donde se almacenaran los datos de la
 *                         partida finalizada (puntos, movimientos, ID).
 * @return JUGADOR_GANO, DERROTA o MEMORIA_LLENA segun el resultado.
 */
int AdministrarJuego(const char *nombreJugador, int idJugador,
                     tTabla *tablaJugadores, tRegistroPartida *partidaOut);

/**
 * @brief Despacha la operacion solicitada sobre el ranking de jugadores.
 *
 *  Segun el codigo de operacion recibido, invoca la funcion de mostrar
 *  el ranking o simplemente registra un nuevo jugador.
 *
 * @param operacion       Codigo de operacion: MOSTRAR o AGREGADO.
 * @param tablaJugadores  Puntero a la tabla indexada de jugadores.
 * @return 0 si la operacion se completo correctamente.
 */
int AdministrarRanking(int operacion, tTabla *tablaJugadores);

/**
 * @brief Ejecuta una ronda completa del juego: turno del jugador y turnos de bandidos.
 *
 *  Si el jugador debe omitir turno, lo indica y saltea su movimiento.
 *  De lo contrario, solicita la direccion de movimiento, tira el dado y
 *  encola los turnos del jugador y los bandidos. Al procesar cada turno,
 *  verifica victoria o derrota y retorna el estado correspondiente.
 *
 * @param jue        Puntero al estado actual del juego.
 * @param jug        Puntero al jugador actual.
 * @param partida    Puntero al tablero donde se ejecutan los movimientos.
 * @param idJugador  Identificador unico del jugador para el registro de partida.
 * @param partidaOut Puntero donde se almacenan los datos de la partida
 *                   si esta finaliza (victoria o derrota).
 * @return JUGADOR_GANO, DERROTA, JUEGO_CONTINUA o MEMORIA_LLENA.
 */
int Jugar(tJuego* jue, tJugador* jug, tTablero* partida,
          int idJugador, tRegistroPartida *partidaOut);

#endif
