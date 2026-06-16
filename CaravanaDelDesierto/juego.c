/**
 * @file juego.c
 * @brief Implementación del motor principal del juego.
 * @details Este archivo contiene la lógica central que controla el flujo del juego,
 *          incluyendo la inicialización, ejecución de turnos, captura de bandidos
 *          y finalización de la partida.
 */

#include <stdio.h>
#include <stdlib.h>

#include "juego.h"
#include "tablero.h"
#include "jugador.h"
#include "bandido.h"

/**
 * @brief Simula el lanzamiento de un dado.
 * @return Un número entero aleatorio entre 1 y DADO (inclusive).
 */
int tirarDado()
{
	return (rand() % DADO) + 1;
}

/**
 * @brief Inicializa la estructura del juego con la configuración proporcionada.
 * @param juego Puntero a la estructura tJuego a inicializar.
 * @param j Puntero al jugador que participará en el juego.
 * @param c Puntero a la configuración del tablero y los bandidos.
 * @return OK si la inicialización fue exitosa, o un código de error en caso contrario.
 */
int iniciarJuego(tJuego *juego, tJugador *j, const tConfig *c)
{
	int code;
	tTablero t;
	tBandido *b;

	// Verificar que la configuración es válida antes de proceder con la inicialización.
	if ((code = verificarCapacidad(&(c->tCfg))) != OK) {
		return code;
	}

	juego->jugador = j;

	juego->bandido =
		(tBandido *)malloc(sizeof(tBandido) * c->tCfg.maxBands);
	if (!juego->bandido) {
		return -3;
	}
	b = juego->bandido;
	for (; b < juego->bandido + c->tCfg.maxBands; b++) {
		crearBandido(b);
	}

	crearTablero(&t, c->tCfg.cantCasillas);
	if ((code = distribuirCasillas(&t, &(c->tCfg), juego->jugador,
				       juego->bandido)) != OK) {
		return code;
	}

	juego->cfg = *c;
	juego->tablero = t;
	juego->turno = 0;
	juego->cantBandidosActivos = c->tCfg.maxBands;
	
	return OK;
}

/**
 * @brief Verifica si un bandido capturó al jugador.
 * @param j Puntero constante al jugador.
 * @param b Puntero constante al bandido.
 * @return 1 si el bandido y el jugador comparten posición y el jugador
 *         no es invulnerable, 0 en caso contrario.
 */
static int hayCaptura(const tJugador *j, const tBandido *b)
{
	return (j->pos == b->pos && j->invulnerable != 1);
}

/**
 * @brief Busca un bandido por su identificador en el arreglo de bandidos activos.
 * @param juego Puntero constante a la estructura del juego.
 * @param id Identificador del bandido a buscar.
 * @return El índice del bandido en el arreglo si se encuentra, -1 en caso contrario.
 */
static int buscarBandidoPorId(const tJuego *juego, int id)
{
	int i;
	for (i = 0; i < juego->cantBandidosActivos; i++) {
		if (juego->bandido[i].id == id) {
			return i;
		}
	}
	return -1;
}

/**
 * @brief Aplica la penalización por captura de un bandido al jugador.
 * @details Le quita una vida al jugador, lo reubica en la casilla inicial,
 *          elimina al bandido del tablero y lo remueve del arreglo de bandidos activos.
 * @param juego Puntero a la estructura del juego.
 * @param idxBandido Índice del bandido que capturó al jugador.
 */
static void aplicarCaptura(tJuego *juego, int posJugadorEnTablero)
{
	tJugador *j = juego->jugador;

	quitarVida(j);
	quitarOmitirTurno(j);
	colocarJugadorEnPos(j, 0);
	hacerInvulnerable(j);
	printf("[DEBUG captura] Reubicando jugador a pos=0 (posAnt ahora=%d)\n",
	       j->posAnterior);
	quitarJugadorDePos(&(juego->tablero), posJugadorEnTablero);
	ponerJugadorEnPosTablero(&(juego->tablero), 0);
}

/**
 * @brief Mueve al jugador según el turno y verifica capturas con los bandidos.
 * @param juego Puntero a la estructura del juego.
 * @param t Puntero constante al turno con la información de movimiento.
 */
static void moverYActualizarJugador(tJuego *juego, const tTurno *t)
{
	int i;
	tJugador *j = juego->jugador;
	tCasilla tc;

	/*
	printf("[DEBUG turnoJugador] mov=%d tablero.cant=%d\n", t->mov,
	       juego->tablero.cant);
	*/
	moverJugador(j, t->mov, juego->tablero.cant);

	listaCircularDobleMirarEnPos(&juego->tablero.casillas, &tc,
				     sizeof(tCasilla), j->pos);

	for (i = 0; i < juego->cantBandidosActivos; i++) {
		if (hayCaptura(j, &juego->bandido[i])) {
			int last;
			/*
			printf("[DEBUG turnoJugador] CAPTURA con bandido idx=%d\n",
			       i);
			*/
			quitarBandidoDePos(&(juego->tablero),
					   juego->bandido[i].pos);
			last = juego->cantBandidosActivos - 1;
			if (i != last) {
				/*
				printf("[DEBUG captura] Swapping bandidos: idx=%d <-> last=%d\n",
				       i, last);
				*/
				tBandido aux = juego->bandido[last];
				juego->bandido[last] = juego->bandido[i];
				juego->bandido[i] = aux;
			}
			juego->cantBandidosActivos--;
			/*
			printf("[DEBUG captura] cantBandidosActivos ahora=%d\n",
			       juego->cantBandidosActivos);
			*/
			aplicarCaptura(juego, j->posAnterior);
			return;
		}
	}

	switch (tc.base) {
	case CASILLA_TORMENTA:
		omitirTurno(j);
		break;
	case CASILLA_OASIS:
		hacerInvulnerable(j);
		break;
	case CASILLA_PREMIO:
		obtenerPunto(j);
		consumirCasilla(&(juego->tablero), j->pos);
		break;
	case CASILLA_VIDA:
		obtenerVida(j);
		consumirCasilla(&(juego->tablero), j->pos);
		break;
	}

	/*
	printf("[DEBUG turnoJugador] sincronizando jugador pos=%d posAnt=%d\n",
	       obtenerPosJugador(j), obtenerPosAnteriorJugador(j));
	*/
	sincronizarJugadorEnTablero(j, &(juego->tablero));
	/*
	printf("[DEBUG turnoJugador] fin turno\n");
	*/
}

/**
 * @brief Mueve un bandido específico y verifica si captura al jugador.
 * @param juego Puntero a la estructura del juego.
 * @param t Puntero constante al turno con el identificador y movimiento del bandido.
 */
static void moverYActualizarBandido(tJuego *juego, const tTurno *t)
{
	int idx = buscarBandidoPorId(juego, t->id);
	tJugador *j = juego->jugador;
	tBandido *b;

	if (idx < 0) {
		return;
	}

	b = &juego->bandido[idx];
	/*
	printf("[DEBUG turnoBandido] id=%d mov=%d tablero.cant=%d\n", t->id,
	       t->mov, juego->tablero.cant);
	*/
	moverBandido(b, t->mov, juego->tablero.cant);
	if (hayCaptura(j, b)) {
		if (j->invulnerable) {
			quitarInvulnerable(j);
		} else {
			/*
			printf("[DEBUG turnoBandido] CAPTURA!\n");
			*/
			aplicarCaptura(juego, j->pos);
		}
		eliminarBandidoDeTablero(&(juego->tablero), b);
		if (idx != juego->cantBandidosActivos - 1) {
			tBandido aux =
				juego->bandido[juego->cantBandidosActivos - 1];
			/*
			printf("[DEBUG captura] Swapping bandidos: idx=%d <-> last=%d\n",
			       idx, juego->cantBandidosActivos - 1);
			*/
			juego->bandido[juego->cantBandidosActivos - 1] = *b;
			*b = aux;
		}
		juego->cantBandidosActivos--;
		/*
		printf("[DEBUG captura] cantBandidosActivos ahora=%d\n",
		       juego->cantBandidosActivos);
		*/
		return;
	}

	/*
	printf("[DEBUG turnoBandido] sincronizando bandido pos=%d posAnt=%d\n",
	       obtenerPosBandido(b), obtenerPosAnteriorBandido(b));
	*/
	sincronizarBandidoEnTablero(b, &(juego->tablero));
	/*
	printf("[DEBUG turnoBandido] fin turno\n");
	*/
}

/**
 * @brief Ejecuta un turno del juego, ya sea del jugador o de un bandido.
 * @param juego Puntero a la estructura del juego.
 * @param t Puntero al turno a ejecutar.
 * @return 1 si el jugador llegó a la última casilla (ganó el juego), 0 en caso contrario.
 */
int correrTurno(tJuego *juego, tTurno *t)
{
	//Si el turno es de jugador
	if (t->tipo == EVT_JUGADOR) {
		moverYActualizarJugador(juego, t);
		if (obtenerPosJugador(juego->jugador) ==
		    juego->tablero.cant - 1) {
			return 1;
		}
		return 0;
	}
	//Si es de bandido:
	moverYActualizarBandido(juego, t);
	return 0;
}

/**
 * @brief Incrementa el contador de turnos del juego.
 * @param j Puntero a la estructura del juego.
 */
void terminarTurno(tJuego *j)
{
	j->turno++;
}

/**
 * @brief Finaliza el juego y libera los recursos asociados.
 * @param juego Puntero constante a la estructura del juego.
 */
void terminarJuego(const tJuego *juego);

int cargarConfiguracion(const char *nombreArchivo, tConfig *cfg)
{
	FILE *f;
	char linea[TAM_LINEA];
	char* iterador;
	int valor;

	f = fopen(nombreArchivo, "rt");
	if (!f) {
		return -1;
	}
	while (fgets(linea,sizeof(linea),f)) {
        iterador = strrchr(linea,'\n');
        if(iterador)
        {
            *iterador = '\0';
        }
        iterador = strrchr(linea,':');
        valor = atoi((iterador+2));
        *iterador = '\0';
		if (strcmp(linea, "cantidad_posiciones") == 0) {
			cfg->tCfg.cantCasillas = valor;
		} else if (strcmp(linea, "vidas_inicio") == 0) {
			cfg->vidasInicio = valor;
			cfg->tCfg.vidasInicio = valor;
		} else if (strcmp(linea, "maximo_bandidos") == 0) {
			cfg->tCfg.maxBands = valor;
		} else if (strcmp(linea, "maximo_premios") == 0) {
			cfg->tCfg.maxPrem = valor;
		} else if (strcmp(linea, "maximo_vidas_extra") == 0) {
			cfg->tCfg.maxVida = valor;
		} else if (strcmp(linea, "maximo_oasis") == 0) {
			cfg->tCfg.maxOasis = valor;
		} else if (strcmp(linea, "maximo_tormentas") == 0) {
			cfg->tCfg.maxTor = valor;
		}
	}

	fclose(f);
	return OK;
}

int cargarConfiguracionPorDefecto(const char *nom, const tConfig *cfg)
{
	FILE *f = fopen(nom, "wt");
	if (!f) {
		return -1;
	}

	fprintf(f, "CANT_CASILLAS=%d\n", cfg->tCfg.cantCasillas);
	fprintf(f, "VIDAS_INICIO=%d\n", cfg->vidasInicio);
	fprintf(f, "MAXIMO_BANDIDOS=%d\n", cfg->tCfg.maxBands);
	fprintf(f, "MAXIMO_PREMIOS=%d\n", cfg->tCfg.maxPrem);
	fprintf(f, "MAXIMO_VIDAS_EXTRAS=%d\n", cfg->tCfg.maxVida);
	fprintf(f, "MAXIMO_OASIS=%d\n", cfg->tCfg.maxOasis);
	fprintf(f, "MAXIMO_TORMENTAS=%d\n", cfg->tCfg.maxTor);

	fclose(f);
	return OK;
}
