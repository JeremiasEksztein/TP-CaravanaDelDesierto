#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interfaz.h"
#include "AdministradorDeJuego.h"
#include "usuarios_db.h"

int main()
{
	char opcion;
	tTabla tablaJugadores;
	tRegistroPartida partidaOut = { 0 };
	tRegistroJugador regJugador, foundJugador;
	char nombre[TAM_NOMBRE];
	int idJugador;

	srand(time(NULL));

	tablaCrear(&tablaJugadores, JUGADORES_DB, JUGADORES_IDX,
		   jugadorIndiceCmp, jugadorLeerClave, sizeof(tRegistroJugador),
		   TAM_NOMBRE_JUG);
	if (tablaAbrir(&tablaJugadores) != OK) {
		printf("Error al abrir la base de datos de jugadores.\n");
		printf("Verifique que el directorio tenga permisos de escritura.\n");
		return 1;
	}

	opcion = CrearMenuInicial(MENU_TEXTO, OPCIONES_MENU);
	limpiarBuff();

	while (opcion != SALIR) {
		switch (opcion) {
		case JUGAR:
			printf("Iniciando el juego...\n");
			SolicitarNombreJugador(nombre, TAM_NOMBRE);

			memset(&regJugador, 0, sizeof(regJugador));
			strncpy(regJugador.nombre, nombre, TAM_NOMBRE_JUG - 1);
			regJugador.nombre[TAM_NOMBRE_JUG - 1] = '\0';

			if (tablaBuscar(&tablaJugadores, &regJugador,
					&foundJugador) == OK) {
				idJugador = foundJugador.id;
			} else {
			idJugador = tablaProximoId(&tablaJugadores);
			regJugador.id = idJugador;
			regJugador.partidasJugadas = 0;
			if (tablaIngresar(&tablaJugadores,
						  &regJugador) != OK) {
					printf("Error al registrar nuevo jugador.\n");
					break;
				}
			}

			if (AdministrarJuego(nombre, idJugador, &tablaJugadores,
					     &partidaOut) == JUGADOR_GANO) {
				/* Victoria — persistencia ya manejada en AdministrarJuego */
			}
			break;
		case RANKING:
			printf("Mostrando el ranking...\n");
			AdministrarRanking(MOSTRAR, &tablaJugadores);
			break;
		default:
			printf("Opcion invalida. Por favor, intente de nuevo.\n");
		}
		opcion = CrearMenuInicial(MENU_TEXTO, OPCIONES_MENU);
	}

	tablaCerrar(&tablaJugadores);
	tablaDestruir(&tablaJugadores);
	return 0;
}
