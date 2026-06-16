#include "interfaz.h"
#include <string.h>
#include <stdio.h>

char CrearMenuInicial(const char *menuTexto, const char *opciones)
{
	char opcion = 1;
	while (strchr(opciones, opcion) == NULL) {
		printf("%s\n", menuTexto);
		printf("Seleccione una opción: ");
		scanf(" %c", &opcion);
		while (opcion == '\n') {
			scanf(" %c", &opcion);
		}
		if (strchr(opciones, opcion) == NULL) {
			printf("Opción no válida. Por favor, Ingresa de nuevo.\n");
		}
	}
	return opcion;
}
void MostrarMensajeEsTurnoDeJugador(const char* jugNombre)
{
	printf("---------------------------------\n");
	printf("¡Es tu turno %s!\n",jugNombre);
	printf("---------------------------------\n");
}
void MostrarMensajeOmisionDeTurno(const char* jugNombre)
{
	printf("¡Vaya! Parece que la tormenta te ha atrapado. No puedes jugar este turno.\n");
}
void MostrarMensajeTurnoBandido(int numDado)
{
	printf("¡Es el turno de los Bandidos!\n");
	printf("Ha Sacado: %d En el dado\n", numDado);
}
int SolicitarDireccionDeMovimiento(const char* jugNombre, int numDado)
{
	char caracter = 0;
	printf("Has Sacado: %d En el Dado\n", numDado);
	printf("¿Que direccion pretendes  moverte?...\n\'A\' para Avanzar \n\'R\' para retroceder\n");
	while(caracter != 'A' && caracter != 'R')
	{
		scanf("%c", &caracter);
		while(caracter == '\n')
		{
			scanf("%c", &caracter);
		}
		if(caracter != 'A' && caracter != 'R')
		{
			printf("\nError, Intenta de nuevo\n");
		}
	}
	return caracter == 'A' ? 1 : -1;
}