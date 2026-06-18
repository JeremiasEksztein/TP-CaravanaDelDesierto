/**
 * @file cola.h
 * @brief Cola generica enlazada simple (FIFO) para el juego
 *        "Caravana del Desierto".
 *
 * Implementa una cola que almacena copias de datos genericos mediante
 * nodos enlazados simples. Soporta operaciones de encolar, desencolar,
 * consultar el frente y destruir la estructura.
 */

#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <string.h>

#include "common.h"

/**
 * @struct tNodo
 * @brief Nodo de la cola generica.
 *
 * Cada nodo almacena una copia del dato apuntado y apunta al siguiente
 * nodo de la secuencia.
 */
typedef struct sNodo {
    struct sNodo *sig; /**< Puntero al siguiente nodo de la cola. */
    unsigned n;        /**< Tamanio en bytes del dato almacenado. */
    void *data;        /**< Puntero al dato copiado. */
}tNodo;
/*typedef struct sNodo tNodo;*/

/**
 * @struct tCola
 * @brief Estructura principal de la cola generica.
 *
 * Mantiene punteros al primer y ultimo nodo para permitir
 * inserciones en O(1) por el final y extracciones en O(1)
 * por el frente.
 */
typedef struct {
    tNodo *pri, /**< Puntero al primer nodo (frente de la cola). */
          *ult; /**< Puntero al ultimo nodo (final de la cola). */
} tCola;

/**
 * @brief Inicializa una cola vacia.
 *
 * Debe llamarse antes de cualquier otra operacion sobre la cola.
 *
 * @param c Puntero a la cola a inicializar.
 */
void colaCrear(tCola *c);

/**
 * @brief Encola una copia del dato al final de la cola.
 *
 * Reserva memoria para el nodo y copia los \p n bytes apuntados por
 * \p data. La cola queda propietaria de esa copia.
 *
 * @param c    Puntero a la cola.
 * @param data Puntero al dato origen que se copiara.
 * @param n    Tamanio en bytes del dato a copiar.
 * @return OK si la operacion tuvo exito, ERR si fallo la reserva de memoria.
 */
int colaEncolar(tCola *c, const void *data, unsigned n);

/**
 * @brief Desencola el elemento del frente, copiandolo en \p buf.
 *
 * Extrae el primer nodo de la cola y copia su contenido en el buffer
 * proporcionado. Libera la memoria del nodo removido.
 *
 * @param c   Puntero a la cola.
 * @param buf Puntero al buffer donde se copiara el dato desencolado.
 * @param n   Tamanio en bytes esperado del dato.
 * @return OK si la operacion tuvo exito, ERR si la cola estaba vacia.
 */
int colaDesencolar(tCola *c, void *buf, unsigned n);

/**
 * @brief Consulta el elemento del frente sin extraerlo.
 *
 * Copia el contenido del primer nodo en \p buf sin modificar la cola.
 *
 * @param c   Puntero constante a la cola.
 * @param buf Puntero al buffer donde se copiara el dato del frente.
 * @param n   Tamanio en bytes esperado del dato.
 * @return OK si la operacion tuvo exito, ERR si la cola estaba vacia.
 */
int colaFrente(const tCola *c, void *buf, unsigned n);

/**
 * @brief Determina si la cola no contiene elementos.
 *
 * @param c Puntero constante a la cola.
 * @return TRUE si la cola esta vacia, FALSE en caso contrario.
 */
int colaEstaVacia(const tCola *c);

/**
 * @brief Destruye la cola y libera toda la memoria asociada.
 *
 * Elimina todos los nodos y sus datos, dejando la cola en estado vacio.
 * Despues de llamar esta funcion se puede volver a usar colaCrear().
 *
 * @param c Puntero a la cola a destruir.
 */
void colaDestruir(tCola *c);

#endif
