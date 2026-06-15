#include "AdministradorDeJuego.h"
#include "juego.h"

int AdministrarInicioJuego()
{
    short int retorno;
	tJuego partidaActual;
	tConfig configActual;
	tJugador jugadorActual;

	if (CargarConfiguracionDeTablero(ARCHIVO_CONFIG_TABLERO,
					 &configActual.tCfg) != 0) {
		//MostrarMensajeArchivoNoEncontrado(ARCHIVO_CONFIG_TABLERO);
		CargarConfiguracionPorDefecto(&configActual.tCfg);
	}
	//crearConfig(&configActual, NIVEL_DEFAULT, DIFICULTAD_DEFAULT);
	crearJugador(&jugadorActual, "Jugador1", VIDAS_DEFAULT);
    retorno = iniciarJuego(&partidaActual, &jugadorActual, &configActual);
    if(retorno == JUGADOR_GANO)
    {
        AdministrarDeRanking(AGREGADO, &jugadorActual);
    }
	return 0;
}
int AdministrarRanking(int operacion, void* extras)
{
    switch(operacion)
    {
        case AGREGADO:
            GuardarJugadorEnRanking(extras);
            break;
        case MOSTRAR:
            MostrarRankingDeJugadores();
            break;
    }
}