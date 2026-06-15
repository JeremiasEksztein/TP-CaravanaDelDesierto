#include "AdministradorDeJuego.h"

int AdministrarInicioJuego()
{
    tJuego partidaActual;
    tConfig configActual;
    tJugador jugadorActual;

    //cargarArchivoDeConfiguracion(NOMBRE_ARHCIVO_CONFIGURACION, &configActual);
    //crearConfig(&configActual, NIVEL_DEFAULT, DIFICULTAD_DEFAULT);
    crearJugador(&jugadorActual, "Jugador1", VIDAS_DEFAULT);
    
    return iniciarJuego(&partidaActual,&jugadorActual,&configActual);

}