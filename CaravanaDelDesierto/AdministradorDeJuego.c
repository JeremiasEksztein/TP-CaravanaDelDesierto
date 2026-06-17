#include "AdministradorDeJuego.h"
#include "juego.h"

int AdministrarJuego(const char *nombreJugador, int idJugador,
		     tTabla *tablaJugadores, tRegistroPartida *partidaOut)
{
	short int retorno;
	tJuego partidaActual;
	tConfig configActual;
	tJugador jugadorActual;
	FILE *archPartidas;

	if (cargarConfiguracion(FILE_CONFIG, &configActual) != 0) {
		cargarConfiguracionPorDefecto(FILE_CONFIG, &configActual);
	}
	crearJugador(&jugadorActual, nombreJugador, VIDAS_DEFAULT);
	retorno = iniciarJuego(&partidaActual, &jugadorActual, &configActual);

	mostrarTableroCompacto(&partidaActual.tablero, jugadorActual.pos);
	if (retorno == 0) {
		retorno = JUEGO_CONTINUA;
		while (retorno == JUEGO_CONTINUA) {
			retorno = Jugar(&partidaActual, &jugadorActual,
					&partidaActual.tablero, idJugador,
					partidaOut);
		}
	}

	if (retorno == JUGADOR_GANO || retorno == DERROTA) {
		mostrarRegistroMovimientos(&partidaActual);
		liberarRegistroMovimientos(&partidaActual);

		archPartidas = fopen(PARTIDA_DB, "ab");
		if (archPartidas) {
			size_t written = fwrite(partidaOut,
						sizeof(tRegistroPartida), 1,
						archPartidas);
			fclose(archPartidas);
		} else {
			printf("Error al abrir %s para escritura\n",
			       PARTIDA_DB);
		}

		if (tablaJugadores) {
			tRegistroJugador current, searchKey;
			memset(&searchKey, 0, sizeof(searchKey));
			strncpy(searchKey.nombre, nombreJugador,
				TAM_NOMBRE_JUG - 1);
			searchKey.nombre[TAM_NOMBRE_JUG - 1] = '\0';

		if (tablaBuscar(tablaJugadores, &searchKey, &current) ==
		    OK) {
			current.partidasJugadas++;
			tablaActualizar(tablaJugadores, &searchKey,
					&current);
		}
		}
	}

	free(partidaActual.bandido);
	return retorno;
}

int AdministrarRanking(int operacion, tTabla *tablaJugadores)
{
	switch (operacion) {
	case AGREGADO:
		break;
	case MOSTRAR:
		MostrarRanking(tablaJugadores);
		break;
	}
	return 0;
}

int Jugar(tJuego *jue, tJugador *jug, tTablero *partida, int idJugador,
	  tRegistroPartida *partidaOut)
{
	short int dado;
	int i;
	tCola colaDeTurnos;
	tTurno actual;
	short int banderaDeVictoria = 0;

	tTurno *turnos =
		(tTurno *)malloc(jue->cantBandidosActivos * sizeof(tTurno));

	if (!turnos) {
		return MEMORIA_LLENA;
	}

	colaCrear(&colaDeTurnos);

	if (ConsultarOmisionDeTurno(jug) > 0) {
		MostrarMensajeOmisionDeTurno(jug->name);
		quitarOmitirTurno(jug);
		esperar(1500);
	} else {
		short int movJugador;
		MostrarMensajeEsTurnoDeJugador(jug->name);
		dado = tirarDado();
		movJugador = SolicitarDireccionDeMovimiento(jug->name, dado);
		movJugador *= dado;
		registrarMovimientoJugador(jue, movJugador);
		IniciarElTurnoDelJugador(&actual);
		crearTurnoJugador(&actual, movJugador, partida, jug);
		colaEncolar(&colaDeTurnos, &actual, sizeof(tTurno));
	}

	for (i = 0; i < jue->cantBandidosActivos; i++) {
		dado = tirarDado();
		MostrarMensajeTurnoBandido(dado);
		IniciarElTurnoDelBandido(&actual,
					 obtenerIdBandido(&(jue->bandido[i])));
		crearTurnoBandido(&actual, jue->bandido + i, jug, partida,
				  dado);
		turnos[i] = actual;
	}

	DesordenarVectorDeTurnos(turnos, jue->cantBandidosActivos);
	for (i = 0; i < jue->cantBandidosActivos; i++) {
		colaEncolar(&colaDeTurnos, turnos + i, sizeof(tTurno));
	}
	free(turnos);

	limpiarPantalla();
	mostrarTableroCompacto(partida, jug->pos);
	MostrarEstadoJugador(jug);

	while (!colaEstaVacia(&colaDeTurnos)) {
		colaDesencolar(&colaDeTurnos, &actual, sizeof(tTurno));
		banderaDeVictoria = correrTurno(jue, &actual);

		if (banderaDeVictoria) {
			limpiarPantalla();
			mostrarTableroCompacto(partida, jug->pos);
			MostrarEstadoJugador(jug);
			MostrarMensajeVictoria(jug->name);
			colaDestruir(&colaDeTurnos);
			if (partidaOut) {
				partidaOut->idJugador = idJugador;
				partidaOut->puntosObtenidos = jug->puntos;
				partidaOut->movimientosRealizados =
					jue->cantMovimientos;
			}
			return JUGADOR_GANO;
		}
		if (ConsultarVidasJugador(jug) == 0) {
			limpiarPantalla();
			mostrarTableroCompacto(partida, jug->pos);
			MostrarEstadoJugador(jug);
			MostrarMensajeDerrota(jug->name);
			colaDestruir(&colaDeTurnos);
			if (partidaOut) {
				partidaOut->idJugador = idJugador;
				partidaOut->puntosObtenidos = jug->puntos;
				partidaOut->movimientosRealizados =
					jue->cantMovimientos;
			}
			return DERROTA;
		}

		limpiarPantalla();
		mostrarTableroCompacto(partida, jug->pos);
		MostrarEstadoJugador(jug);
		esperar(500);
	}

	colaDestruir(&colaDeTurnos);
	return JUEGO_CONTINUA;
}
