#include "AdministradorDeJuego.h"

int AdministrarInicioJuego()
{
    tJuego partidaActual;
    tConfig configActual;
    tJugador jugadorActual;

    if(CargarConfiguracionDeTablero(ARCHIVO_CONFIG_TABLERO, &configActual) != 0)
    {
        //MostrarMensajeArchivoNoEncontrado(ARCHIVO_CONFIG_TABLERO);
        CargarConfiguracionPorDefecto(&configActual);
    }
    //crearConfig(&configActual, NIVEL_DEFAULT, DIFICULTAD_DEFAULT);
    crearJugador(&jugadorActual, "Jugador1", VIDAS_DEFAULT);

    return iniciarJuego(&partidaActual,&jugadorActual,&configActual);

}