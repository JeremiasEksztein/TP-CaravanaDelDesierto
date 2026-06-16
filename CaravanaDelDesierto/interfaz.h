#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#include <stdio.h>
#include "jugador.h"
#define MENU_TEXTO "==========================\n"\
                    "Bienvenido a la Caravana del Desierto\n"\
                    "\n1. JUGAR\n"\
                    "\n2. VER RANKING\n"\
                    "\n3. Salir\n"\
                    "\n==========================\n"
#define OPCIONES_MENU "123"
#define SALIR '3'
#define JUGAR '1'
#define RANKING '2'
char CrearMenuInicial(const char* menuTexto, const char* opciones);
void MostrarMensajeEsTurnoDeJugador(const char* jugNombre);
void MostrarMensajeOmisionDeTurno(const char* jugNombre);
void MostrarMensajeTurnoBandido(int numDado);
int SolicitarDireccionDeMovimiento(const char* jugNombre, int numDado);
void MostrarEstadoJugador(const tJugador *j);
void MostrarMensajeDerrota(const char *nombre);
void MostrarMensajeVictoria(const char *nombre);
#endif
