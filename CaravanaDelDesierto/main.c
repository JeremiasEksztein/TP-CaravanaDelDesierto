#include <stdio.h>
#include <stdlib.h>

#include "interfaz.h"
#include "AdministradorDeJuego.h"


int main()
{
    char opcion;

    // En esta parte hay un menú
    opcion = CrearMenuInicial(MENU_TEXTO, OPCIONES_MENU);
    while(opcion!=SALIR)
    {
        switch (opcion) {
        case JUGAR:
            printf("Iniciando el juego...\n");
            // lógica inicio juego
            if(AdministrarInicioJuego()==1)
            {
                //Guardar en ranking;
            }
            //Guardar progreso
            break;
        case RANKING:
            printf("Mostrando el ranking...\n");
            //  lógica para el ranking

            break;
        default:
            printf("Opción inválida. Por favor, intente de nuevo.\n");
        }
        opcion = CrearMenuInicial(MENU_TEXTO, OPCIONES_MENU);
    }
    printf("Hello world!\n");
    return 0;
}
