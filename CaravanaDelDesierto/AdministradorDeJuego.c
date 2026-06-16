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
	unsigned totalDeTurnos = 1 + jue->cantBandidosActivos;
	short int dado;
	int i;
	//Vitales
	tCola colaDeTurnos;
	tTurno actual;
	short int banderaDeVictoria = 0;

	tTurno *turnos = (tTurno *)malloc(totalDeTurnos * sizeof(tTurno));

	if (!turnos) {
		return MEMORIA_LLENA;
	}

	//Siempre el primer turno es del jugador.
	IniciarElTurnoDelJugador(&(turnos[0]));

	for (i = 0; i < jue->cantBandidosActivos; i++) {
		//Suma uno en la posición de turnos, porque el [0] es el jugador
		IniciarElTurnoDelBandido(&(turnos[i + 1]),
					 obtenerIdBandido(&(jue->bandido[i])));
	}
	//Comienza Turno
	//Primero los desordenamos
	DesordenarVectorDeTurnos(turnos, totalDeTurnos);

	colaCrear(&colaDeTurnos);
	//Colocarlos en la cola
	for (i = 0; i < totalDeTurnos; i++) {
		colaEncolar(&colaDeTurnos, turnos + i, sizeof(tTurno));
	}
	free(turnos);
	while (!colaEstaVacia(&colaDeTurnos)) {
		//Aun quedan turnos.
		colaDesencolar(&colaDeTurnos, &actual, sizeof(tTurno));
		if (esTurnoDeJugador(&actual)) {
			//El jugador solo puede hacer algo si no está en estado de omisión
			MostrarMensajeEsTurnoDeJugador(jug->name);
			if (ConsultarOmisionDeTurno(jug) == OMISION) {
				//Realizar un mensaje que explique al jugador que no puede jugar.
				MostrarMensajeOmisionDeTurno(jug->name);
				quitarOmitirTurno(jug);

			} else {
				//Lógica de jugabilidad.
				dado = tirarDado();
				//Logica de eleccion de direccion
				
				actual.dir =  SolicitarDireccionDeMovimiento(jug->name, dado);	
				actual.dir*=dado;
				crearTurnoJugador(&actual, dado, partida, jug);
			}
		} else 
		{
			dado = tirarDado();
			MostrarMensajeTurnoBandido(dado);
			//Hay que buscar el id del correspondiente bandido
			i = BuscarIndiceDeBandido(jue->bandido,actual.id, jue->cantBandidosActivos);
			crearTurnoBandido(&actual, 
				jue->bandido + i,
					  jug, partida, dado);
		}
		banderaDeVictoria = correrTurno(jue, &actual);
		if (banderaDeVictoria) {
			colaDestruir(&colaDeTurnos);
			return JUGADOR_GANO;
		}
		if (ConsultarVidasJugador(jug) == 0) {
			colaDestruir(&colaDeTurnos);
			return DERROTA;
		}
	}

	colaDestruir(&colaDeTurnos);
	return JUEGO_CONTINUA;
}
