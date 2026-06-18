/**
 * @file common.h
 * @brief Definiciones comunes, tipos de funcion y utilidades generales
 *        para el juego "Caravana del Desierto".
 *
 * Centraliza las macros de estado, los punteros a funcion genericos
 * y las funciones de apoyo que comparten el resto de los modulos.
 */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>

/** @def OK
 *  @brief Codigo de retorno exitoso. */
#define OK 0

/** @def ERR
 *  @brief Codigo de retorno erroneo. */
#define ERR 1

/** @def TRUE
 *  @brief Valor booleano verdadero (1). */
#define TRUE 1

/** @def FALSE
 *  @brief Valor booleano falso (0). */
#define FALSE 0

/** @def MIN(x, y)
 *  @brief Retorna el menor de dos valores numericos. */
#define MIN(x, y) ((x) > (y) ? (y) : (x))

/** @def MAX(x, y)
 *  @brief Retorna el mayor de dos valores numericos. */
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * @typedef fnCmp
 * @brief Puntero a funcion comparadora de dos elementos genericos.
 *
 * Debe retornar un valor menor, igual o mayor a cero si el primer
 * argumento es menor, igual o mayor que el segundo, respectivamente.
 *
 * @param a Puntero al primer elemento.
 * @param b Puntero al segundo elemento.
 * @return Negativo, cero o positivo segun la relacion de orden.
 */
typedef int (*fnCmp)(const void *, const void *);

/**
 * @typedef fnImprimir
 * @brief Puntero a funcion que imprime un elemento generico.
 *
 * @param dato Puntero al dato a imprimir.
 */
typedef void (*fnImprimir)(const void *);

/**
 * @typedef fnAccion
 * @brief Puntero a funcion que ejecuta una accion sobre un elemento,
 *        recibiendo contexto adicional del usuario.
 *
 * @param dato     Puntero al dato sobre el que se actua.
 * @param contexto Puntero con informacion adicional proporcionada por el usuario.
 */
typedef void (*fnAccion)(void *, void *);

/**
 * @brief Imprime un mensaje de error en stderr con formato variable.
 *
 * Util para reportar errores del programa sin abortar la ejecucion.
 * Funciona de manera analoga a fprintf(stderr, ...).
 *
 * @param msg Cadena de formato (igual que printf).
 * @param ... Argumentos adicionales segun el formato.
 */
void eprintf(const char *msg, ...);

/**
 * @brief Limpia el buffer de entrada estandar.
 *
 * Descarta todos los caracteres restantes en stdin hasta encontrar
 * un salto de linea o fin de archivo. Se usa tipicamente despues
 * de lecturas parciales con scanf.
 */
void limpiarBuff();

#endif
