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
	if (retorno != 0) {
        //Jugar
		retorno = Jugar(&partidaActual,&jugadorActual);
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
int Jugar(tJuego* jue,tJugador* jug)
{
	/*
	tCola colaDeTurnos;
	colaCrear(&colaDeTurnos);
	tTurno actual;
	tTurno* turnos = (tTurno*)malloc((jue->cantBandidosActivos+1)*sizeof(tTurno));
	if(!turnos)
	{
		return -1;
	}
	turnos[0].id = 0;
	for(int i = 0; i < 1 + jue->cantBandidosActivos ; i++)
	{
		colaEncolar(&colaDeTurnos,turnos+rand()%(1+jue->cantBandidosActivos),sizeof(tTurno));
	}
	//Comienza Turno
	for(int i = 0; i < 1 + jue->cantBandidosActivos ; i++)
	{
		colaDesencolar(&colaDeTurnos,&actual,sizeof(tTurno));
		correrTurno(jue,&actual);
	}
	free(turnos);
	*/
	return 0;
}