/**
 * @file listaCircularDoble.h
 * @brief Lista circular doblemente enlazada generica para el juego
 *        "Caravana del Desierto".
 *
 * Implementa una lista donde cada nodo posee enlace anterior y siguiente,
 * y el ultimo nodo apunta al primero, formando un anillo. Permite
 * insercion, busqueda, actualizacion por posicion, recorrido y destruccion.
 */

#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include <stdlib.h>
#include <string.h>

#include "common.h"

/**
 * @struct tNodo2
 * @brief Nodo de la lista circular doblemente enlazada.
 *
 * Almacena una copia del dato y punteros al nodo anterior y al siguiente,
 * cerrando el ciclo entre el primer y el ultimo nodo.
 */
typedef struct sNodo2 tNodo2;
struct sNodo2 {
  tNodo2 *ant, /**< Puntero al nodo anterior en la lista. */
         *sig; /**< Puntero al nodo siguiente en la lista. */
  unsigned n;  /**< Tamanio en bytes del dato almacenado. */
  void *data;  /**< Puntero al dato copiado. */
};

/**
 * @typedef tListaCircularDoble
 * @brief Puntero al primer nodo de la lista circular doble.
 *
 * Un valor NULL indica que la lista esta vacia.
 */
typedef tNodo2 *tListaCircularDoble;

/**
 * @brief Inicializa una lista circular doble vacia.
 *
 * Debe llamarse antes de cualquier otra operacion sobre la lista.
 *
 * @param l Puntero a la lista a inicializar.
 */
void listaCircularDobleCrear(tListaCircularDoble *l);

/**
 * @brief Inserta una copia del dato al final de la lista.
 *
 * La lista se mantiene circular: el nuevo nodo se enlaza entre el
 * ultimo nodo existente y el primero.
 *
 * @param l    Puntero a la lista.
 * @param data Puntero al dato origen que se copiara.
 * @param n    Tamanio en bytes del dato a copiar.
 * @return OK si la operacion tuvo exito, ERR si fallo la reserva de memoria.
 */
int listaCircularDobleEmpujar(tListaCircularDoble *l, const void *data,
                              unsigned n);

/**
 * @brief Busca un elemento en la lista por clave de comparacion.
 *
 * Recorre la lista utilizando la funcion de comparacion \p cmp hasta
 * encontrar un elemento que sea igual a \p clave. Copia el dato encontrado
 * en \p buf.
 *
 * @param l     Puntero a la lista.
 * @param clave Puntero al dato clave que se desea encontrar.
 * @param buf   Puntero al buffer donde se copiara el dato encontrado.
 * @param n     Tamanio en bytes del dato esperado.
 * @param cmp   Funcion comparadora (fnCmp).
 * @return OK si se encontro el elemento, ERR si no existe en la lista.
 */
int listaCircularDobleBuscar(tListaCircularDoble *l, const void *clave,
                             void *buf, unsigned n, fnCmp cmp);

/**
 * @brief Actualiza el dato en una posicion especifica de la lista.
 *
 * Aplica la funcion \p accion sobre el nodo en la posicion \p pos,
 * permitiendo modificar el contenido almacenado con contexto adicional.
 *
 * @param l      Puntero a la lista.
 * @param data   Puntero al dato de contexto que se pasa a la accion.
 * @param pos    Indice (base 0) del nodo a actualizar.
 * @param accion Funcion que recibe el dato del nodo y el contexto.
 * @return OK si la posicion es valida, ERR si esta fuera de rango.
 */
int listaCircularDobleActualizarEnPos(tListaCircularDoble *l, const void *data,
                                      int pos, fnAccion accion);

/**
 * @brief Retorna la cantidad de nodos de la lista.
 *
 * @param l Puntero constante a la lista.
 * @return Cantidad de elementos, 0 si la lista esta vacia.
 */
unsigned listaCircularDobleLargo(const tListaCircularDoble *l);

/**
 * @brief Destruye la lista y libera toda la memoria asociada.
 *
 * Elimina todos los nodos y sus datos, dejando la lista en estado vacio.
 * Despues de llamar esta funcion se puede volver a usar
 * listaCircularDobleCrear().
 *
 * @param l Puntero a la lista a destruir.
 */
void listaCircularDobleDestruir(tListaCircularDoble *l);

/**
 * @brief Busca la posicion de un elemento en la lista.
 *
 * Recorre la lista con la funcion \p cmp y retorna el indice
 * (base 0) de la primera coincidencia.
 *
 * @param l   Puntero a la lista.
 * @param d   Puntero al dato clave a buscar.
 * @param cmp Funcion comparadora (fnCmp).
 * @return Indice del elemento encontrado, o -1 si no existe.
 */
int listaCircularDobleBuscarPos(tListaCircularDoble *l, const void *d,
                                fnCmp cmp);

/**
 * @brief Recorre la lista de izquierda a derecha aplicando una accion.
 *
 * Itera todos los nodos desde el primero hasta volver al inicio,
 * invocando la funcion \p print sobre cada dato almacenado.
 *
 * @param l     Puntero constante a la lista.
 * @param print Funcion de impresion o accion a aplicar sobre cada nodo.
 * @return OK si la operacion tuvo exito, ERR si la lista esta vacia.
 */
int listaCircularDobleMostrarLR(const tListaCircularDoble *l,
                                const fnAccion print);

/**
 * @brief Obtiene el dato almacenado en una posicion especifica.
 *
 * Copia en \p buff el contenido del nodo ubicado en la posicion \p pos.
 *
 * @param l    Puntero a la lista.
 * @param buff Puntero al buffer donde se copiara el dato.
 * @param tam  Tamanio en bytes del dato esperado.
 * @param pos  Indice (base 0) del nodo cuyo dato se desea obtener.
 * @return OK si la posicion es valida, ERR si esta fuera de rango.
 */
int listaCircularDobleMirarEnPos(tListaCircularDoble *l, void *buff,
                                 const unsigned tam, int pos);
#endif
