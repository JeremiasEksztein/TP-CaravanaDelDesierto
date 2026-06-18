/**
 * @file juego.h
 * @brief Funciones principales del motor de juego "Caravana del Desierto".
 *
 * Este archivo declara las constantes de configuracion por defecto y las
 * funciones que controlan el ciclo de vida de una partida: inicializacion,
 * ejecucion de turnos, verificacion de fin de juego y liberacion de recursos.
 */

#ifndef JUEGO_H
#define JUEGO_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bandido.h"
#include "cola.h"
#include "jugador.h"
#include "tablero.h"
#include "turnos.h"
#include "jugador.h"
#include "interfaz.h"
#include "bandido.h"
/**
 * @def FILE_CONFIG
 * @brief Nombre del archivo de configuración del juego.
 *
 * Define la ruta al archivo de texto que contiene los parámetros
 * de configuración iniciales del juego (cantidad de casillas, bandidos, etc.).
 */
#define FILE_CONFIG "config.txt"

 /* Sencilla configuracion por defecto, para testing: */

/**
 * @def CANT_CASILLAS_DEFAULT
 * @brief Cantidad de casillas del tablero por defecto para testing.
 */
#define CANT_CASILLAS_DEFAULT 20

/**
 * @def CANT_BANDIDOS_DEFAULT
 * @brief Cantidad de bandidos por defecto para testing.
 */
#define CANT_BANDIDOS_DEFAULT 3

/**
 * @def CANT_OASIS_DEFAULT
 * @brief Cantidad de casillas de oasis por defecto para testing.
 */
#define CANT_OASIS_DEFAULT 3

/**
 * @def CANT_PREMIO_DEFAULT
 * @brief Cantidad de casillas con premio por defecto para testing.
 */
#define CANT_PREMIO_DEFAULT 2

/**
 * @def CANT_VIDA_DEFAULT
 * @brief Cantidad de casillas con vida extra por defecto para testing.
 */
#define CANT_VIDA_DEFAULT 2

/**
 * @def CANT_TOR_DEFAULT
 * @brief Cantidad de casillas de tormenta por defecto para testing.
 */
#define CANT_TOR_DEFAULT 2

/**
 * @def CANT_VIDAS_INICIO_DEFAULT
 * @brief Cantidad de vidas iniciales del jugador por defecto para testing.
 */
#define CANT_VIDAS_INICIO_DEFAULT 3

/**
 * @struct tConfig
 * @brief Configuración general del juego.
 *
 * Contiene las vidas iniciales del jugador y la configuración
 * específica del tablero dentro de una misma estructura.
 */
typedef struct {
	int vidasInicio;       /**< Cantidad de vidas iniciales del jugador. */
	tConfigTablero tCfg;   /**< Configuración del tablero (casillas, bandidos, etc.). */
} tConfig;

/**
 * @struct tJuego
 * @brief Estado completo de una partida de "Caravana del Desierto".
 *
 * Agrupa todos los componentes del juego: los bandidos, el jugador,
 * el tablero, la configuración, el turno actual y el registro
 * de movimientos realizados.
 */
typedef struct {
	tBandido* bandido;          /**< Array dinámico de bandidos activos. */
	tJugador* jugador;          /**< Puntero al jugador controlado por el usuario. */
	tTablero tablero;           /**< Tablero circular donde se desarrolla el juego. */
	tConfig cfg;                /**< Configuración de la partida. */
	int cantBandidosActivos;    /**< Cantidad de bandidos actualmente en juego. */
	int turno;                  /**< Número de turno actual. */
	int cantMovimientos;        /**< Cantidad total de movimientos realizados. */
	tCola registroMovimientos;  /**< Cola con el historial de movimientos del jugador. */
} tJuego;

/**
 * @def FILE_CONFIG
 * @brief Nombre del archivo de configuracion que se lee al iniciar el juego.
 */
#define FILE_CONFIG "config.txt"

 /**
	* @def DADO
	* @brief Cantidad de caras del dado utilizado para determinar los movimientos.
	*/
#define DADO 6

	/**
	 * @def VIDAS_DEFAULT
	 * @brief Cantidad de vidas iniciales por defecto si no se especifica en la
	 * configuracion.
	 */
#define VIDAS_DEFAULT 3

	 /**
		* @def CANT_POSICIONES_DEFAULT
		* @brief Cantidad de casillas del tablero por defecto si no se especifica en la
		* configuracion.
		*/
#define CANT_POSICIONES_DEFAULT 25

		/**
		 * @def MAX_BANDIDOS_DEFAULT
		 * @brief Cantidad maxima de bandidos en el tablero por defecto.
		 */
#define MAX_BANDIDOS_DEFAULT 3

		 /**
			* @def MAX_PREMIOS_DEFAULT
			* @brief Cantidad maxima de premios en el tablero por defecto.
			*/
#define MAX_PREMIOS_DEFAULT 5

			/**
			 * @def MAX_VIDAS_EXTRAS_DEFAULT
			 * @brief Cantidad maxima de vidas extras en el tablero por defecto.
			 */
#define MAX_VIDAS_EXTRAS_DEFAULT 1

			 /**
				* @def MAX_OASIS_DEFAULT
				* @brief Cantidad maxima de oasis en el tablero por defecto.
				*/
#define MAX_OASIS_DEFAULT 1

				/**
				 * @def MAX_TORMENTAS_DEFAULT
				 * @brief Cantidad maxima de tormentas en el tablero por defecto.
				 */
#define MAX_TORMENTAS_DEFAULT 2

				 /**
					* @brief Inicializa el estado del juego y del jugador a partir de la
					* configuracion.
					*
					* Carga la configuracion, genera el tablero con sus casillas especiales,
					* inicializa los bandidos y posiciona al jugador en la casilla de inicio.
					*
					* @param juego Puntero al estado del juego a inicializar.
					* @param j Puntero al jugador a inicializar.
					* @param c Puntero a la configuracion de la partida.
					* @return 1 si la inicializacion fue exitosa, 0 en caso de error.
					*/
int iniciarJuego(tJuego* juego, tJugador* j, const tConfig* c);

/**
 * @brief Ejecuta todas las acciones correspondientes al turno actual.
 *
 * Cada turno consta del movimiento del jugador seguido del movimiento
 * de cada uno de los bandidos activos. Al finalizar, se evalua si
 * ocurrio algun evento especial (colision, premio, oasis, tormenta).
 *
 * @param juego Puntero al estado del juego.
 */
void correrTurnoJuego(tJuego* juego);

/**
 * @brief Verifica si la partida ha finalizado.
 *
 * El juego termina cuando el jugador llega a la meta (victoria) o
 * cuando se queda sin vidas (derrota).
 *
 * @param juego Puntero constante al estado del juego.
 * @return 1 si el juego termino (por victoria o derrota), 0 si aun continua.
 */
int juegoTermino(const tJuego* juego);

/**
 * @brief Libera los recursos y muestra el resultado final de la partida.
 *
 * Debe llamarse una vez que juegoTermino() devuelve 1. Libera la memoria
 * asignada a los bandidos y muestra el mensaje de victoria o derrota.
 *
 * @param juego Puntero constante al estado del juego.
 */
void terminarJuego(const tJuego* juego);

/**
 * @brief Procesa un movimiento individual dentro del turno.
 *
 * Ejecuta el desplazamiento de un jugador o un bandido sobre el tablero
 * y resuelve las consecuencias del movimiento (colisiones, casillas
 * especiales).
 *
 * @param juego Puntero al estado del juego.
 * @param t Puntero a la estructura que describe el movimiento a realizar.
 * @return 1 si el movimiento se realizo correctamente, 0 en caso de error.
 */
int correrTurno(tJuego* juego, tTurno* t);

/**
 * @brief Simula la tirada del dado de DADO caras.
 *
 * Genera un numero aleatorio entre 1 y DADO inclusive, que representa
 * la cantidad de casillas que el jugador o un bandido debe avanzar.
 *
 * @return Un numero entero aleatorio entre 1 y DADO.
 */
int tirarDado(void);

/**
 * @brief Carga la configuración del juego desde un archivo de texto.
 *
 * Lee los parámetros de configuración (cantidad de casillas, bandidos,
 * oasis, etc.) desde el archivo especificado y los almacena en la
 * estructura de configuración dada.
 *
 * @param nombreArchivo Ruta al archivo de configuración.
 * @param cfg Puntero a la estructura donde se almacenará la configuración.
 * @return 1 si la carga fue exitosa, 0 en caso de error.
 */
int cargarConfiguracion(const char* nombreArchivo, tConfig* cfg);

/**
 * @brief Carga una configuración por defecto del juego.
 *
 * Asigna valores predeterminados a todos los parámetros de configuración,
 * útil para testing o cuando no existe un archivo de configuración válido.
 *
 * @param nom Nombre del archivo de configuración (se ignora si no existe).
 * @param cfg Puntero a la estructura donde se almacenará la configuración.
 * @return 1 si la carga fue exitosa, 0 en caso de error.
 */
int cargarConfiguracionPorDefecto(const char* nom, tConfig* cfg);

/**
 * @brief Inicializa la cola de registro de movimientos del juego.
 *
 * Crea y prepara la cola que almacenará el historial de movimientos
 * realizados por el jugador a lo largo de la partida.
 *
 * @param juego Puntero al estado del juego cuyo registro se inicializará.
 */
void inicializarRegistroMovimientos(tJuego* juego);

/**
 * @brief Registra un movimiento del jugador en el historial.
 *
 * Agrega la cantidad de casillas movidas al registro de movimientos
 * de la partida actual.
 *
 * @param juego Puntero al estado del juego.
 * @param mov Cantidad de casillas que se movió el jugador.
 */
void registrarMovimientoJugador(tJuego* juego, int mov);

/**
 * @brief Muestra el registro completo de movimientos del jugador.
 *
 * Recorre la cola de movimientos y muestra cada entrada registrada
 * durante la partida.
 *
 * @param juego Puntero al estado del juego cuyo registro se mostrará.
 */
void mostrarRegistroMovimientos(tJuego* juego);

/**
 * @brief Libera la memoria del registro de movimientos.
 *
 * Destruye la cola de movimientos y libera todos los recursos
 * asociados al historial de la partida.
 *
 * @param juego Puntero al estado del juego cuyo registro se liberará.
 */
void liberarRegistroMovimientos(tJuego* juego);
#endif
