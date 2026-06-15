#include "tablero.h"
#include "jugador.h"
#include "bandido.h"
#include <stdlib.h>
#include <stdio.h>

void cambiarANormal(void *ctx, const void *d)
{
	tCasilla *c = (tCasilla *)ctx;
	if (c->base == CASILLA_JUGADOR || c->base == CASILLA_BANDIDO) {
		c->base = CASILLA_NORMAL;
	}
	c->hayJugador = 0;
	c->hayBandido = 0;
}

void cambiarAJugador(void *ctx, const void *d)
{
	tCasilla *c = (tCasilla *)ctx, *datos = (tCasilla *)d;
	if (c->base == CASILLA_NORMAL) {
		c->base = datos->base;
	}
	c->hayJugador = datos->hayJugador;
}

void cambiarABandido(void *ctx, const void *d)
{
	tCasilla *c = (tCasilla *)ctx, *datos = (tCasilla *)d;
	if (c->base == CASILLA_NORMAL) {
		c->base = datos->base;
	}
	c->hayBandido = datos->hayBandido;
}

void prnt(const void *d)
{
	tCasilla tc = *(tCasilla *)d;
	if (tc.hayJugador && tc.base != CASILLA_JUGADOR) {
		printf("%c %c\t\n", tc.base, CASILLA_JUGADOR);
	} else if (tc.hayBandido) {
		printf("%c\t\n", CASILLA_BANDIDO);
	} else {
		printf("%c\t\n", tc.base);
	}
}

/*Solo se debe usar al inicio de la partida*/
static int ponerCasilla(tCasilla *c, double_list_t *l)
{
	int code;

	code = list_push_last(l, c, sizeof(tCasilla));

	if (code != OK) {
		printf("Error");
	}

	return code;
}

/*
 * Crea la casilla al principio del juego
 */
static void crearCasilla(tCasilla *c, const tTipoCasilla tipo)
{
	if (tipo == CASILLA_INICIO) {
		c->hayJugador = 1;
	} else {
		c->hayJugador = 0;
	}
	if (tipo == CASILLA_BANDIDO) {
		c->hayBandido = 1;
		c->base = CASILLA_NORMAL;
	} else {
		c->hayBandido = 0;
		c->base = tipo;
	}
}

void crearTablero(tTablero *t, int nCasillas)
{
	t->cant = nCasillas;
	list_create(&(t->casillas));
}

int verificarCapacidad(const tConfigTablero *cfg)
{
	if (cfg->cantCasillas < cfg->maxBands + cfg->maxTor + cfg->maxVida +
					cfg->maxPrem + cfg->maxOasis) {
		perror("Cantidad de objetos es mayor a las casillas disponibles");
		return -1;
	}
	return OK;
}

int distribuirCasillas(tTablero *t, const tConfigTablero *cfg, tJugador *j,
		       tBandido *b)
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

	tCasilla c;

	crearCasilla(&c, CASILLA_INICIO);
	ponerCasilla(&c, &(t->casillas));
	colocarJugadorEnPos(j, 0);

	for (; i < cfg->cantCasillas - 1; i++) {
		total = restO + restT + restV + restP + restB + restN;
		int r = rand() % total;

		if (r < restO) {
			crearCasilla(&c, CASILLA_OASIS);
			restO--;
		} else if (r < restO + restT) {
			crearCasilla(&c, CASILLA_TORMENTA);
			restT--;
		} else if (r < restO + restT + restV) {
			crearCasilla(&c, CASILLA_VIDA);
			restV--;
		} else if (r < restO + restT + restV + restP) {
			crearCasilla(&c, CASILLA_PREMIO);
			restP--;
		} else if (r < restO + restT + restV + restP + restB) {
			crearCasilla(&c, CASILLA_BANDIDO);
			colocarBandidoEnPos(b, i);
			b++;
			restB--;
		} else {
			crearCasilla(&c, CASILLA_NORMAL);
			restN--;
		}
		ponerCasilla(&c, &(t->casillas));
	}

	crearCasilla(&c, CASILLA_META);
	ponerCasilla(&c, &(t->casillas));

	list_show_lr(&(t->casillas), prnt);

	return OK;
}

int cmpJugador(const void *a, const void *b)
{
	tCasilla *ctx = (tCasilla *)a, *d = (tCasilla *)b;
	if (ctx->base == d->base || ctx->hayJugador == d->hayJugador) {
		return 0;
	}
	return -1;
}

int cmpBandido(const void *a, const void *b)
{
	tCasilla *ctx = (tCasilla *)a, *d = (tCasilla *)b;
	if (ctx->base == d->base || ctx->hayBandido == d->hayBandido) {
		return 0;
	}
	return -1;
}

void moverPiezaDeTablero(tTipoCasilla tc, void *pieza, tTablero *t)
{
	tCasilla c, aux;
	aux.base = CASILLA_BANDIDO;
	aux.hayBandido = 1;
	aux.hayJugador = 0;

	c.base = tc;
	if (tc == CASILLA_JUGADOR) {
		tJugador *j = (tJugador *)pieza;
		c.hayJugador = 1;
		list_update_by_pos(&(t->casillas), &c,
				   obtenerPosAnteriorJugador(j),
				   cambiarANormal);
		list_update_by_pos(&(t->casillas), &c, obtenerPosJugador(j),
				   cambiarAJugador);
	} else if (tc == CASILLA_BANDIDO) {
		tBandido *b = (tBandido *)pieza;
		c.hayBandido = 1;
		list_update_by_pos(&(t->casillas), &c,
				   obtenerPosAnteriorBandido(b),
				   cambiarANormal);
		list_update_by_pos(&(t->casillas), &c, obtenerPosBandido(b),
				   cambiarABandido);
	}
}

void mostrarTablero(const tTablero *t)
{
	list_show_lr(&(t->casillas), prnt);
}

int buscarJugadorEnTablero(tTablero *t)
{
	tCasilla c;
	c.base = CASILLA_JUGADOR;
	c.hayJugador = 1;
	c.hayBandido = 0;
	return list_search_pos(&(t->casillas), &c, cmpJugador);
}

void eliminarBandidoDeTablero(tTablero *t, tBandido *b)
{
	tCasilla c;
	c.base = CASILLA_NORMAL;
	c.hayBandido = 0;
	c.hayJugador = 0;
	list_update_by_pos(&t->casillas, &c, b->pos, cambiarANormal);
}
//Extras
int CargarConfiguracionDeTablero(const char *nombreArchivo, tConfigTablero *cfg)
{
	int cantidadDeApariciones;
	char* letra;
	char linea[TAM_LINEA];
	FILE* arch = fopen(nombreArchivo, "rt");
	if(!arch)
	{
		return ERROR_ARCHIVO;
	}
	//Inicializo los valores
	cfg->cantCasillas = 0;
	cfg->maxBands = 0;
	cfg->maxOasis = 0;
	cfg->maxPrem = 0;
	cfg->maxVida = 0;
	cfg->maxTor = 0;
	//Ahora cargo el archivo:
	while(fgets(linea,sizeof(linea), arch))
	{
		cfg->cantCasillas++;
		
		letra = strchr(linea, 'O');
		if(letra)
		{
			cfg->maxOasis;
		}
		letra = strchr(linea,'P')
		if(letra)
		{
			cfg->maxPrem++;
		}
		letra = strchr(linea,'T')
		if(letra)
		{
			cfg->maxTor++;
		}
		
		cantidadDeApariciones = ContarOcurrencias(linea,'B');
		if(cantidadDeApariciones && !(ES_PRIMERA_LINEA))
		{
			cfg->maxBands+=cantidadDeApariciones;
		}
	}
	fclose(arch);
	return 0;
}
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
int CargarConfiguracionPorDefecto(tConfigTablero *cfg)
{
	cfg->cantCasillas = CANT_CASILLAS_DEFAULT;
	cfg->maxBands = CANT_BANDIDOS_DEFAULT;
	cfg->maxOasis = CANT_OASIS_DEFAULT;
	cfg->maxPrem = CANT_PREMIO_DEFAULT;
	cfg->maxVida = CANT_VIDA_DEFAULT;
	cfg->maxTor = CANT_TOR_DEFAULT;
	return OK;
}