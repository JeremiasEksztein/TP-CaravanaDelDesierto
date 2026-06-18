/**
 * @file interfaz.h
 * @brief Funciones de interfaz de usuario para el juego "Caravana del Desierto".
 *
 * Este modulo agrupa todas las rutinas que manejan la interaccion con el
 * jugador por consola: menus, mensajes de estado, solicitud de datos y
 * presentacion de resultados. Las macros definidas aqui controlan el
 * texto y las opciones del menu principal.
 */

#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#include <stdio.h>
#include "jugador.h"
#include "usuarios_db.h"

/** @def MENU_TEXTO
 *  @brief Cadena de formato con el texto completo del menu principal.
 *
 *  Contiene el banner de bienvenida y las opciones enumeradas que se
 *  muestran al iniciar el programa. */
#define MENU_TEXTO "==========================\n"\
                    "Bienvenido a la Caravana del Desierto\n"\
                    "\n1. JUGAR\n"\
                    "\n2. VER RANKING\n"\
                    "\n3. Salir\n"\
                    "\n==========================\n"

/** @def OPCIONES_MENU
 *  @brief Cadena con los caracteres validos como opcion del menu principal.
 *
 *  Cada caracter representa una seleccion valida: '1' (Jugar),
 *  '2' (Ranking) y '3' (Salir). */
#define OPCIONES_MENU "123"

/** @def SALIR
 *  @brief Caracter que representa la opcion "Salir" del menu principal. */
#define SALIR '3'

/** @def JUGAR
 *  @brief Caracter que representa la opcion "Jugar" del menu principal. */
#define JUGAR '1'

/** @def RANKING
 *  @brief Caracter que representa la opcion "Ver Ranking" del menu principal. */
#define RANKING '2'

/**
 * @brief Muestra un menu por consola y solicita una opcion valida al usuario.
 *
 *  Presenta el texto del menu y lee caracteres hasta que el usuario ingresa
 *  uno que pertenezca a la cadena de opciones validas.
 *
 * @param menuTexto Cadena con el texto del menu a mostrar.
 * @param opciones  Cadena con los caracteres validos de seleccion.
 * @return El caracter ingresado por el usuario que coincide con una opcion valida.
 */
char CrearMenuInicial(const char* menuTexto, const char* opciones);

/**
 * @brief Muestra un mensaje indicando que es el turno del jugador.
 *
 * @param jugNombre Nombre del jugador cuyo turno comienza.
 */
void MostrarMensajeEsTurnoDeJugador(const char* jugNombre);

/**
 * @brief Muestra un mensaje indicando que el jugador debe omitir su turno.
 *
 * Se utiliza cuando el jugador cae en una casilla de tormenta y no puede
 * moverse en la ronda actual.
 *
 * @param jugNombre Nombre del jugador afectado.
 */
void MostrarMensajeOmisionDeTurno(const char* jugNombre);

/**
 * @brief Muestra un mensaje indicando que el jugador perdio una vida.
 *
 * Se invoca cuando un bandido alcanza al jugador y este pierde una vida.
 *
 * @param jugNombre Nombre del jugador que perdio la vida.
 */
void MostrarMensajePerdidaDeVida(const char* jugNombre);

/**
 * @brief Muestra el mensaje del turno de los bandidos junto con el valor del dado.
 *
 * @param numDado Valor obtenido al tirar el dado para los bandidos.
 */
void MostrarMensajeTurnoBandido(int numDado);

/**
 * @brief Solicita al jugador la direccion de movimiento tras tirar el dado.
 *
 *  Pregunta al jugador si desea avanzar ('A') o retroceder ('R') y
 *  valida la entrada hasta recibir un caracter valido.
 *
 * @param jugNombre Nombre del jugador (reservado para mensajes personalizados).
 * @param numDado    Valor del dado que indica cuantas casillas desplazarse.
 * @return 1 si el jugador elige avanzar, -1 si elige retroceder.
 */
int SolicitarDireccionDeMovimiento(const char* jugNombre, int numDado);

/**
 * @brief Muestra por consola el estado actual del jugador.
 *
 *  Imprime nombre, vidas, puntos y estados especiales como invulnerabilidad
 *  u omision de turno.
 *
 * @param j Puntero constante al jugador cuyo estado se desea mostrar.
 */
void MostrarEstadoJugador(const tJugador* j);

/**
 * @brief Muestra un mensaje de derrota cuando el jugador pierde todas sus vidas.
 *
 * @param nombre Nombre del jugador derrotado.
 */
void MostrarMensajeDerrota(const char* nombre);

/**
 * @brief Muestra un mensaje de victoria cuando el jugador llega a la meta.
 *
 * @param nombre Nombre del jugador victorioso.
 */
void MostrarMensajeVictoria(const char* nombre);

/**
 * @brief Solicita al jugador que ingrese su nombre por consola.
 *
 *  Lee una cadena de caracteres alphanumericos hasta que la entrada sea
 *  valida: no vacia, sin caracteres especiales y dentro del limite de longitud.
 *
 * @param nombre  Buffer donde se almacenara el nombre ingresado.
 * @param maxLen  Tamaño maximo del buffer (incluyendo el terminador nulo).
 */
void SolicitarNombreJugador(char* nombre, int maxLen);

#endif
