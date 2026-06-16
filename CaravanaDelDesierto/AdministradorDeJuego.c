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
        
	}
    //AdministrarRanking(AGREGADO, &jugadorActual);
	return 0;
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
