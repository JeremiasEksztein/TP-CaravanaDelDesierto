#include "interfaz.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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

void MostrarEstadoJugador(const tJugador *j)
{
	printf("---------------------------------\n");
	printf(" Jugador: %s\n", j->name);
	printf(" Vidas: %d\n", j->vidas);
	printf(" Puntos: %d\n", j->puntos);
	if (j->invulnerable > 0)
		printf(" Estado: INMUNE\n");
	if (j->omitirTurno > 0)
		printf(" Estado: TURNO OMITIDO\n");
	printf("---------------------------------\n");
}

void MostrarMensajeDerrota(const char *nombre)
{
	printf("\n=================================\n");
	printf("  %s HA MUERTO EN EL DESIERTO\n", nombre);
	printf("=================================\n");
}

void MostrarMensajeVictoria(const char *nombre)
{
	printf("\n=================================\n");
	printf("  %s HA LLEGADO A LA META!\n", nombre);
	printf("=================================\n");
}

void SolicitarNombreJugador(char *nombre, int maxLen)
{
	int i, valid;

	do {
		valid = 1;
		printf("Ingrese su nombre (max %d caracteres alfanumericos): ", maxLen - 1);
		scanf(" %10s", nombre);
		limpiarBuff();

		for(i = 0; nombre[i] && i < maxLen; i++) {
			if(!isalnum((unsigned char)nombre[i])) {
				valid = 0;
				printf("Nombre invalido. Solo se permiten letras y numeros.\n");
				break;
			}
		}

		if(valid && strlen(nombre) == 0) {
			valid = 0;
			printf("El nombre no puede estar vacio.\n");
		}
	} while(!valid);
}

