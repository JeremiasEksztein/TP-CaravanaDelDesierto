#ifndef TABLERO_H
#define TABLERO_H

#include "bandido.h"
#include "jugador.h"
#include "listaCircularDoble.h"

#include <string.h>

#define FILE_TABLERO "caravana.txt"
#define CASILLA_NORMAL '.'
#define CASILLA_OASIS 'O'
#define CASILLA_TORMENTA 'T'
#define CASILLA_PREMIO 'P'
#define CASILLA_VIDA 'V'
#define CASILLA_BANDIDO 'B'
#define CASILLA_META 'S'
#define CASILLA_INICIO 'I'
#define CASILLA_JUGADOR 'J'
#define OK 0
/* Otros defines */
#define ERROR_ARCHIVO 4
#define TAM_LINEA 150
#define ES_PRIMERA_LINEA cfg->cantCasillas == 1

/**
 * @typedef tTipoCasilla
 * @brief Tipo numerico para identificar el tipo de casilla.
 */
typedef unsigned char tTipoCasilla;
typedef unsigned char tTipoPieza;

/**
 * @struct tCasilla
 * @brief Representa una casilla individual del tablero.
 *
 * Cada casilla tiene un tipo base (normal, oasis, tormenta, etc.),
 * una pieza que la ocupa (jugador, bandido o vacio) y un contador
 * de bandidos presentes.
 */
typedef struct {
	tTipoCasilla base; /**< Tipo base de la casilla (CASILLA_NORMAL,
												CASILLA_OASIS, etc.) */
	tTipoPieza pieza;  /**< Pieza que ocupa la casilla ('J', 'B' o 0 si vacia) */
	int cantBandidosEnCasilla; /**< Cantidad de bandidos actualmente en esta
																casilla */
} tCasilla;

typedef struct {
	tListaCircularDoble casillas;
	int cant;
} tTablero;

typedef struct {
	int cantCasillas;
	int maxBands;
	int maxPrem;
	int maxVida;
	int maxOasis;
	int maxTor;
	int vidasInicio;
} tConfigTablero;

void crearTablero(tTablero* t, int nCasillas);
int distribuirCasillas(tTablero* t, const tConfigTablero* cfg, tJugador* j,
											 tBandido* b);
void moverPiezaDeTablero(tTipoCasilla tc, void* pieza, tTablero* t);
int verificarCapacidad(const tConfigTablero* cfg);
int buscarJugadorEnTablero(tTablero* t);
void mostrarTablero(const tTablero* t);
void mostrarTableroCompacto(const tTablero* t, int posJugador);
void limpiarPantalla(void);
void esperar(unsigned int ms);

void eliminarBandidoDeTablero(tTablero* t, tBandido* b);
/**
 * @brief Decrementa en uno el contador de bandidos de una casilla.
 *
 * Si la cantidad de bandidos llega a cero, elimina la pieza de la casilla.
 * Util para reflejar la eliminacion o salida de un bandido de una posicion.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla de la que se quita el bandido.
 */
void quitarBandidoDePos(tTablero* t, int pos);

/**
 * @brief Quita al jugador de una casilla especifica del tablero.
 *
 * Elimina la pieza del jugador ('J') de la casilla indicada, dejandola
 * con su tipo base original.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla de la que se quita al jugador.
 */
void quitarJugadorDePos(tTablero* t, int pos);

/**
 * @brief Coloca al jugador en una casilla especifica del tablero.
 *
 * Asigna la pieza del jugador ('J') a la casilla indicada.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla donde se coloca al jugador.
 */
void ponerJugadorEnPosTablero(tTablero* t, int pos);

/**
 * @brief Consume el efecto especial de una casilla, devolviendola a estado
 * normal.
 *
 * Transforma el tipo base de la casilla en la posicion indicada a
 * CASILLA_NORMAL. Se utiliza despues de que el jugador haya obtenido
 * un premio o una vida extra en esa casilla.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla a consumir.
 */
void consumirCasilla(tTablero* t, int pos);

/**
 * @brief Sincroniza la posicion del jugador en el tablero.
 *
 * Quita al jugador de su posicion anterior y lo coloca en la actual.
 * Maneja correctamente casillas con bandidos.
 *
 * @param j Puntero al jugador a sincronizar.
 * @param t Puntero al tablero donde se realiza el movimiento.
 */
void sincronizarJugadorEnTablero(tJugador* j, tTablero* t);

/**
 * @brief Sincroniza la posicion de un bandido en el tablero.
 *
 * Quita el bandido de su posicion anterior (decrementando contador) y
 * lo coloca en la nueva (incrementando contador).
 *
 * @param b Puntero al bandido a sincronizar.
 * @param t Puntero al tablero donde se realiza el movimiento.
 */
void sincronizarBandidoEnTablero(tBandido* b, tTablero* t);

/* OTRAS FUNCIONES AUXILIARES PARA EL MANEJO DEL TABLERO: */
int CargarConfiguracionDeTablero(const char* nombreArchivo,
																 tConfigTablero* cfg);

int ContarOcurrencias(const char* cadena, char caracter);
int CargarConfiguracionPorDefecto(tConfigTablero* cfg);
#endif
