#if defined(__linux__) || defined(__gnu_linux__)
#define _POSIX_C_SOURCE 199309L
#endif

#include "tablero.h"
#include "jugador.h"
#include "bandido.h"
#include <stdlib.h>
#include <stdio.h>


#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

void esperar(unsigned int ms)
{
#ifdef _WIN32
	Sleep(ms);
#else
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (long)(ms % 1000) * 1000000L;
	nanosleep(&ts, NULL);
#endif
}

void limpiarPantalla(void)
{
	#ifdef _WIN32
	system("cls");
	#else
	   printf("\033[2J\033[H");
       fflush(stdout);
	#endif
}


/* Wrappers */
static int casillaPonerJugador(tCasilla* c)
{
	if (c->pieza == CASILLA_JUGADOR) {
		return 0;
	}
	c->pieza = CASILLA_JUGADOR;
	return 1;
}

static int casillaQuitarJugador(tCasilla* c)
{
	if (c->pieza == CASILLA_JUGADOR) {
		c->pieza = 0;
		return 1;
	}
	return 0;
}

static void casillaPonerBandido(tCasilla* c)
{
	c->cantBandidosEnCasilla++;
	c->pieza = CASILLA_BANDIDO;
}

static int casillaQuitarBandido(tCasilla* c)
{
	c->cantBandidosEnCasilla--;
	if (c->cantBandidosEnCasilla == 0) {
		c->pieza = 0;
		return 0;
	}
	return 1;
}

static void wrapperQuitarJugador(void* ctx, void* d)
{
	(void)d;
	casillaQuitarJugador((tCasilla*)ctx);
}

static void wrapperPonerJugador(void* ctx, void* d)
{
	(void)d;
	casillaPonerJugador((tCasilla*)ctx);
}

static void wrapperQuitarBandido(void* ctx, void* d)
{
	(void)d;
	casillaQuitarBandido((tCasilla*)ctx);
}

static void wrapperPonerBandido(void* ctx, void* d)
{
	(void)d;
	casillaPonerBandido((tCasilla*)ctx);
}

static void wrapperConsumirCasilla(void* ctx, void* d)
{
	tCasilla* c;
	(void)d;
	c = (tCasilla*)ctx;
	c->base = CASILLA_NORMAL;
}

/**
 * @brief Consume el efecto especial de una casilla, devolviendola a estado normal.
 *
 * Transforma el tipo base de la casilla en la posicion indicada a
 * CASILLA_NORMAL mediante el callback wrapperConsumirCasilla.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla a consumir.
 */
void consumirCasilla(tTablero* t, int pos)
{
	listaCircularDobleActualizarEnPos(&t->casillas, NULL, pos,
																		wrapperConsumirCasilla);
}

/**
 * @brief Decrementa en uno el contador de bandidos de una casilla.
 *
 * Si la cantidad de bandidos llega a cero, elimina la pieza de la casilla
 * utilizando el callback wrapperQuitarBandido.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla de la que se quita el bandido.
 */
void quitarBandidoDePos(tTablero* t, int pos)
{
	listaCircularDobleActualizarEnPos(&t->casillas, NULL, pos,
																		wrapperQuitarBandido);
}

/**
 * @brief Quita al jugador de una casilla especifica del tablero.
 *
 * Elimina la pieza del jugador ('J') de la casilla indicada mediante
 * el callback wrapperQuitarJugador.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla de la que se quita al jugador.
 */
void quitarJugadorDePos(tTablero* t, int pos)
{
	listaCircularDobleActualizarEnPos(&t->casillas, NULL, pos,
																		wrapperQuitarJugador);
}

/**
 * @brief Coloca al jugador en una casilla especifica del tablero.
 *
 * Asigna la pieza del jugador ('J') a la casilla indicada utilizando
 * el callback wrapperPonerJugador.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla donde se coloca al jugador.
 */
void ponerJugadorEnPosTablero(tTablero* t, int pos)
{
	listaCircularDobleActualizarEnPos(&t->casillas, NULL, pos,
																		wrapperPonerJugador);
}

/**
 * @brief Imprime el estado de una casilla por consola.
 *
 * Funcion de callback utilizada por list_show_lr() para recorrer
 * y mostrar cada casilla del tablero.
 *
 * Si la casilla esta ocupada muestra el caracter de la pieza entre
 * corchetes, de lo contrario muestra un punto. En ambos casos se
 * imprime el tipo base y la cantidad de bandidos.
 *
 * @param d Puntero a la casilla (@ref tCasilla) a imprimir.
 */
void prnt(void* d, void* aux)
{
	tCasilla tc;
	(void)aux;
	if (!d) {
		return;
	}
	tc = *(tCasilla*)d;
	if (tc.pieza) {
		printf("[%c] base=%c bandidos=%d \n", tc.pieza, tc.base,
					 tc.cantBandidosEnCasilla);
	}
	else {
		printf(" .  base=%c bandidos=%d \n", tc.base,
					 tc.cantBandidosEnCasilla);
	}
}

/*Solo se debe usar al inicio de la partida*/
static int ponerCasilla(tCasilla* c, tListaCircularDoble* l)
{
	int code;

	code = listaCircularDobleEmpujar(l, c, sizeof(tCasilla));

	if (code != OK) {
		printf("Error");
	}

	return code;
}

/**
 * @brief Inicializa los campos de una casilla segun su tipo.
 *
 * Configura la pieza inicial y el contador de bandidos en funcion
 * del tipo de casilla especificado. Las casillas de inicio reciben
 * la pieza del jugador. Las casillas de bandido inician con un
 * bandido y mantienen como base el tipo CASILLA_NORMAL.
 *
 * @param c Puntero a la casilla (@ref tCasilla) a inicializar.
 * @param tipo Tipo de casilla a crear (usando las constantes @ref tTipoCasilla).
 */
 /*
	* Crea la casilla al principio del juego
	*/
static void crearCasilla(tCasilla* c, const tTipoCasilla tipo)
{
	if (tipo == CASILLA_INICIO) {
		c->cantBandidosEnCasilla = 0;
		c->base = tipo;
		c->pieza = CASILLA_JUGADOR;
	}
	else if (tipo == CASILLA_BANDIDO) {
		c->cantBandidosEnCasilla = 1;
		c->pieza = CASILLA_BANDIDO;
		c->base = CASILLA_NORMAL;
	}
	else {
		c->cantBandidosEnCasilla = 0;
		c->pieza = 0;
		c->base = tipo;
	}
}

void crearTablero(tTablero* t, int nCasillas)
{
	t->cant = nCasillas;
	listaCircularDobleCrear(&(t->casillas));
}

int verificarCapacidad(const tConfigTablero* cfg)
{
	if (cfg->cantCasillas < cfg->maxBands + cfg->maxTor + cfg->maxVida +
			cfg->maxPrem + cfg->maxOasis) {
		perror("Cantidad de objetos es mayor a las casillas disponibles");
		return -1;
	}

	return OK;
}

int distribuirCasillas(tTablero* t, const tConfigTablero* cfg, tJugador* j,
											 tBandido* b)
{
	int i = 1;
	int restO = cfg->maxOasis;
	int restT = cfg->maxTor;
	int restV = cfg->maxVida;
	int restP = cfg->maxPrem;
	int restB = cfg->maxBands;
	int restN = (cfg->cantCasillas - 2) -
		(restO + restT + restV + restP + restB);
	int total = restO + restT + restV + restP + restB + restN;
	int r;

	tCasilla c;

	crearCasilla(&c, CASILLA_INICIO);
	ponerCasilla(&c, &(t->casillas));
	colocarJugadorEnPos(j, 0);

	for (; i < cfg->cantCasillas - 1; i++) {
		total = restO + restT + restV + restP + restB + restN;
		r = rand() % total;

		if (r < restO) {
			crearCasilla(&c, CASILLA_OASIS);
			restO--;
		}
		else if (r < restO + restT) {
			crearCasilla(&c, CASILLA_TORMENTA);
			restT--;
		}
		else if (r < restO + restT + restV) {
			crearCasilla(&c, CASILLA_VIDA);
			restV--;
		}
		else if (r < restO + restT + restV + restP) {
			crearCasilla(&c, CASILLA_PREMIO);
			restP--;
		}
		else if (r < restO + restT + restV + restP + restB) {
			crearCasilla(&c, CASILLA_BANDIDO);
			colocarBandidoEnPos(b, i);
			b++;
			restB--;
		}
		else {
			crearCasilla(&c, CASILLA_NORMAL);
			restN--;
		}
		ponerCasilla(&c, &(t->casillas));
	}

	crearCasilla(&c, CASILLA_META);
	ponerCasilla(&c, &(t->casillas));

	return OK;
}

/**
 * @brief Funcion de comparacion para buscar al jugador en una casilla.
 *
 * Compara dos casillas y devuelve 0 si ambas contienen la pieza del
 * jugador ('J'). Es utilizada como criterio de busqueda por
 * list_search_pos().
 *
 * @param a Puntero a la primera casilla (@ref tCasilla) a comparar.
 * @param b Puntero a la segunda casilla (@ref tCasilla) a comparar.
 * @return 0 si ambas casillas tienen al jugador, -1 en caso contrario.
 */
int cmpJugador(const void* a, const void* b)
{
	tCasilla* ctx = (tCasilla*)a, * d = (tCasilla*)b;
	if (ctx->pieza == d->pieza && ctx->pieza == 'J') {
		return 0;
	}
	return -1;
}
void guardarTablero(const tTablero* t, int posJugador, const char* nombreArchivo)
{
	int i, b;
	tCasilla c;
	char base_char;
	int has_jugador, n_bandidos;
	FILE* f;

	if (t == NULL || t->cant == 0) {
		return;
	}

	f = fopen(nombreArchivo, "wt");
	if (f == NULL) {
		return;
	}

	for (i = 0; i < t->cant; i++) {
		if (listaCircularDobleMirarEnPos(
			(tListaCircularDoble*)&(t->casillas), &c,
			sizeof(tCasilla), i) != OK) {
			break;
		}

		base_char = (c.base != CASILLA_NORMAL) ? (char)c.base : '.';
		has_jugador = (i == posJugador);
		n_bandidos = c.cantBandidosEnCasilla;

		fprintf(f, "[%c", base_char);

		if (has_jugador)
			fprintf(f, " J");

		for (b = 0; b < n_bandidos; b++)
			fprintf(f, " B");

		if (!has_jugador && n_bandidos == 0)
			fprintf(f, " .");

		fprintf(f, "]");
	}
	fprintf(f, "\n");
	fclose(f);
}

/**
 * @brief Funcion de comparacion para buscar una casilla con bandidos.
 *
 * Compara dos casillas y devuelve 0 si tienen el mismo tipo base o la
 * misma cantidad de bandidos. Es utilizada como criterio de busqueda
 * por list_search_pos().
 *
 * @param a Puntero a la primera casilla (@ref tCasilla) a comparar.
 * @param b Puntero a la segunda casilla (@ref tCasilla) a comparar.
 * @return 0 si las casillas coinciden en tipo base o cantidad de bandidos,
 *         -1 en caso contrario.
 */
int cmpBandido(const void* a, const void* b)
{
	tCasilla* ctx = (tCasilla*)a, * d = (tCasilla*)b;
	if (ctx->base == d->base ||
			ctx->cantBandidosEnCasilla == d->cantBandidosEnCasilla) {
		return 0;
	}
	return -1;
}

void mostrarTableroCompacto(const tTablero* t, int posJugador)
{
	int i, b;
	tCasilla c;
	char base_char;
	int has_jugador, n_bandidos;

	if (t == NULL || t->cant == 0) {
		return;
	}

	for (i = 0; i < t->cant; i++) {
		if (listaCircularDobleMirarEnPos(
			(tListaCircularDoble*)&(t->casillas), &c,
			sizeof(tCasilla), i) != OK) {
			break;
		}

		base_char = (c.base != CASILLA_NORMAL) ? (char)c.base : '.';
		has_jugador = (i == posJugador);
		n_bandidos = c.cantBandidosEnCasilla;

		printf("[%c", base_char);

		if (has_jugador)
			printf(" J");

		for (b = 0; b < n_bandidos; b++)
			printf(" B");

		if (!has_jugador && n_bandidos == 0)
			printf(" .");

		printf("]");
	}
	printf("\n");
}

void mostrarTablero(const tTablero* t)
{
	mostrarTableroCompacto(t, -1);
}

static void sincronizarPieza(tTablero* t, int posAnterior, int posActual,
														 tTipoCasilla tipo)
{
	if (posAnterior != posActual) {
		if (tipo == CASILLA_JUGADOR) {
			listaCircularDobleActualizarEnPos(&(t->casillas), NULL,
																				posAnterior,
																				wrapperQuitarJugador);
		}
		else {
			listaCircularDobleActualizarEnPos(&(t->casillas), NULL,
																				posAnterior,
																				wrapperQuitarBandido);
		}
	}

	if (tipo == CASILLA_JUGADOR) {
		listaCircularDobleActualizarEnPos(
			&(t->casillas), NULL, posActual, wrapperPonerJugador);
	}
	else {
		listaCircularDobleActualizarEnPos(
			&(t->casillas), NULL, posActual, wrapperPonerBandido);
	}
}

/**
 * @brief Sincroniza la posicion del jugador en el tablero.
 *
 * Quita al jugador de su posicion anterior y lo coloca en la actual.
 * Maneja correctamente casillas con bandidos.
 *
 * @param j Puntero al jugador a sincronizar.
 * @param t Puntero al tablero (@ref tTablero) donde se realiza el
 *        movimiento.
 */
void sincronizarJugadorEnTablero(tJugador* j, tTablero* t)
{
	int posAnt = obtenerPosAnteriorJugador(j);
	int posAct = obtenerPosJugador(j);
	sincronizarPieza(t, posAnt, posAct, CASILLA_JUGADOR);
}

/**
 * @brief Sincroniza la posicion de un bandido en el tablero.
 *
 * Quita el bandido de su posicion anterior (decrementando contador) y
 * lo coloca en la nueva (incrementando contador).
 *
 * @param b Puntero al bandido a sincronizar.
 * @param t Puntero al tablero (@ref tTablero) donde se realiza el
 *        movimiento.
 */
void sincronizarBandidoEnTablero(tBandido* b, tTablero* t)
{
	int posAnt = obtenerPosAnteriorBandido(b);
	int posAct = obtenerPosBandido(b);
	sincronizarPieza(t, posAnt, posAct, CASILLA_BANDIDO);
}

/**
 * @brief Elimina a un bandido del tablero, restaurando su casilla al estado
 * normal.
 *
 * Utiliza la posicion anterior del bandido para localizar la casilla
 * que ocupaba y la restaura mediante casillaQuitarBandido().
 *
 * @param t Puntero al tablero (@ref tTablero) del cual se elimina el bandido.
 * @param b Puntero al bandido (@ref tBandido) a eliminar del tablero.
 */
void eliminarBandidoDeTablero(tTablero* t, tBandido* b)
{
	listaCircularDobleActualizarEnPos(&t->casillas, NULL, b->posAnterior,
																		wrapperQuitarBandido);
}

/* Extras */

int ContarOcurrencias(const char* cadena, char caracter)
{
	int contador = 0;

	if (cadena == NULL) {
		return 0;
	}

	while (*cadena != '\0') {
		if (*cadena == caracter) {
			contador++;
		}
		cadena++;
	}

	return contador;
}
