#include "AdministradorDeJuego.h"
#include "juego.h"

int AdministrarJuego()
{
	short int retorno;
	tJuego partidaActual;
	tConfig configActual;
	tJugador jugadorActual;

	if (cargarConfiguracion(FILE_CONFIG, &configActual) != 0) {
		//MostrarMensajeArchivoNoEncontrado(ARCHIVO_CONFIG_TABLERO);
		cargarConfiguracionPorDefecto(FILE_CONFIG, &configActual);
	}
	//crearConfig(&configActual, NIVEL_DEFAULT, DIFICULTAD_DEFAULT);
	crearJugador(&jugadorActual, "Jugador1", VIDAS_DEFAULT);
	retorno = iniciarJuego(&partidaActual, &jugadorActual, &configActual);

	mostrarTableroCompacto(&partidaActual.tablero, jugadorActual.pos);
	if (retorno == 0) {
		retorno = JUEGO_CONTINUA;
		while (retorno == JUEGO_CONTINUA) {
			//Jugar
			retorno = Jugar(&partidaActual, &jugadorActual,
					&partidaActual.tablero);
		}
	}
	//AdministrarRanking(AGREGADO, &jugadorActual);
	return retorno;
}

int AdministrarRanking(int operacion, void *extras)
{
	switch (operacion) {
	case AGREGADO:
		GuardarJugadorEnRanking(extras);
		break;
	case MOSTRAR:
		MostrarRankingDeJugadores();
		break;
	}
	return 0;
}

int Jugar(tJuego *jue, tJugador *jug, tTablero *partida)
{
	//Auxiliares
	short int dado;
	int i;
	//Vitales
	tCola colaDeTurnos;
	tTurno actual;
	short int banderaDeVictoria = 0;

	tTurno *turnos = (tTurno *)malloc(jue->cantBandidosActivos * sizeof(tTurno));

	if (!turnos) {
		return MEMORIA_LLENA;
	}

	colaCrear(&colaDeTurnos);

	/* ---------- Fase 1: crear todos los turnos ---------- */

	/* Turno del jugador (si no está omitido) — siempre primero */
	if (ConsultarOmisionDeTurno(jug) > 0) {
		MostrarMensajeOmisionDeTurno(jug->name);
		quitarOmitirTurno(jug);
		esperar(1500);
	} else {
		MostrarMensajeEsTurnoDeJugador(jug->name);
		dado = tirarDado();
		actual.dir = SolicitarDireccionDeMovimiento(jug->name, dado);
		actual.dir *= dado;
		IniciarElTurnoDelJugador(&actual);
		crearTurnoJugador(&actual, actual.dir, partida, jug);
		colaEncolar(&colaDeTurnos, &actual, sizeof(tTurno));
	}

	/* Turnos de los bandidos (se desordenan entre sí) */
	for (i = 0; i < jue->cantBandidosActivos; i++) {
		dado = tirarDado();
		MostrarMensajeTurnoBandido(dado);
		IniciarElTurnoDelBandido(&actual,
					 obtenerIdBandido(&(jue->bandido[i])));
		crearTurnoBandido(&actual, jue->bandido + i, jug, partida,
				  dado);
		turnos[i] = actual;
	}

	/* Fase 2: desordenar bandidos y encolar */
	DesordenarVectorDeTurnos(turnos, jue->cantBandidosActivos);
	for (i = 0; i < jue->cantBandidosActivos; i++) {
		colaEncolar(&colaDeTurnos, turnos + i, sizeof(tTurno));
	}
	free(turnos);

	/* Fase 3: ejecutar turnos uno por uno */
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
			return JUGADOR_GANO;
		}
		if (ConsultarVidasJugador(jug) == 0) {
			limpiarPantalla();
			mostrarTableroCompacto(partida, jug->pos);
			MostrarEstadoJugador(jug);
			MostrarMensajeDerrota(jug->name);
			colaDestruir(&colaDeTurnos);
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
