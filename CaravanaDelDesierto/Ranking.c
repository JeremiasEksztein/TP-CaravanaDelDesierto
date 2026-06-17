#include "Ranking.h"
#include "interfaz.h"

#define MAX_RANKING 100

static int cmpRankingDesc(const void *a, const void *b)
{
	const tNodoRanking *na = (const tNodoRanking *)a;
	const tNodoRanking *nb = (const tNodoRanking *)b;

	if (na->puntosAcumulados > nb->puntosAcumulados) return -1;
	if (na->puntosAcumulados < nb->puntosAcumulados) return 1;
	/* Desempate compuesto: ID + nombre */
	if (na->id != nb->id) return na->id - nb->id;
	return strcmp(na->nombre, nb->nombre);
}

/* Contexto para imprimir filas del ranking */
typedef struct {
	int pos;
} tCtxImprimir;

static void imprimirFilaRanking(void *d, void *ctx)
{
	tNodoRanking *nodo = (tNodoRanking *)d;
	tCtxImprimir *c = (tCtxImprimir *)ctx;

	printf("%d. %-10s - %d pts | %d turnos | ID:%d\n",
	       c->pos++, nodo->nombre, nodo->puntosAcumulados,
	       nodo->movimientosAcumulados, nodo->id);
}

/* Contexto para cargar nombres desde el arbol indexado */
typedef struct {
	char nombresPorId[MAX_RANKING][11];
} tCtxNombres;

static void cargarNombreDesdeIndice(void *data, void *ctx)
{
	tEntradaIndice *entrada = (tEntradaIndice *)data;
	tCtxNombres *c = (tCtxNombres *)ctx;
	int id = entrada->id;

	if (id >= 0 && id < MAX_RANKING) {
		strncpy(c->nombresPorId[id], entrada->clave, 10);
		c->nombresPorId[id][10] = '\0';
	}
}

int MostrarRanking(tTabla *tablaJugadores)
{
	FILE *archPar;
	tRegistroPartida partida;
	tArbolBinBusq arbolRanking;
	tCtxImprimir ctxImp;
	tCtxNombres ctxNombres;
	int puntosPorId[MAX_RANKING] = {0};
	int movsPorId[MAX_RANKING] = {0};
	int idsVistos[MAX_RANKING];
	int totalJugadores = 0;
	int i;

	/* Fase 1: recorrer partidas.dat y acumular */
	archPar = fopen(PARTIDA_DB, "rb");
	if (!archPar) {
		printf("No hay partidas registradas.\n");
		return OK;
	}

	while (fread(&partida, sizeof(tRegistroPartida), 1, archPar) == 1) {
		int id = partida.idJugador;
		if (id < 0 || id >= MAX_RANKING)
			continue;

		if (puntosPorId[id] == 0 && movsPorId[id] == 0) {
			idsVistos[totalJugadores++] = id;
		}
		puntosPorId[id] += partida.puntosObtenidos;
		movsPorId[id] += partida.movimientosRealizados;
	}
	fclose(archPar);

	if (totalJugadores == 0) {
		printf("No hay partidas registradas.\n");
		return OK;
	}

	/* Fase 2: cargar nombres desde el arbol indexado */
	memset(&ctxNombres, 0, sizeof(ctxNombres));
	if (tablaJugadores) {
		arbolBinBusqVisitar(&tablaJugadores->indice, E_INORDEN,
				    cargarNombreDesdeIndice, &ctxNombres);
	}

	/* Fase 3: insertar en arbol ordenado por puntos */
	arbolBinBusqCrear(&arbolRanking);
	for (i = 0; i < totalJugadores; i++) {
		int id = idsVistos[i];
		tNodoRanking nodo;

		nodo.nombre[0] = '\0';
		if (id >= 0 && id < MAX_RANKING &&
		    ctxNombres.nombresPorId[id][0] != '\0') {
			strncpy(nodo.nombre, ctxNombres.nombresPorId[id], 10);
			nodo.nombre[10] = '\0';
		}
		if (nodo.nombre[0] == '\0') {
			snprintf(nodo.nombre, 11, "ID:%d", id);
		}

		nodo.id = id;
		nodo.puntosAcumulados = puntosPorId[id];
		nodo.movimientosAcumulados = movsPorId[id];

		arbolBinBusqPoner(&arbolRanking, &nodo, sizeof(tNodoRanking),
				  cmpRankingDesc);
	}

	/* Fase 4: mostrar */
	ctxImp.pos = 1;
	printf("\n===== RANKING DE JUGADORES =====\n");
	arbolBinBusqVisitar(&arbolRanking, E_INORDEN, imprimirFilaRanking,
			    &ctxImp);
	printf("================================\n");

	arbolBinBusqDestruir(&arbolRanking);
	return OK;
}
