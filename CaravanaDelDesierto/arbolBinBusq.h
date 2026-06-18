/**
 * @file arbolBinBusq.h
 * @brief Arbol binario de busqueda generico para el juego
 *        "Caravana del Desierto".
 *
 * Implementa un ABB que almacena copias de datos genericos y los
 * organiza segun una funcion de comparacion. Soporta insercion,
 * extraccion, busqueda, recorrido en tres ordenes y destruccion.
 */

#ifndef ARBOLBINBUSQ_H_INCLUDED
#define ARBOLBINBUSQ_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#include "common.h"

/**
 * @enum eTipoRecorrido
 * @brief Define los tipos de recorrido disponibles para el arbol.
 *
 * E_INORDEN   -> izquierdo, raiz, derecho.
 * E_POSTORDEN  -> izquierdo, derecho, raiz.
 * E_PREORDEN   -> raiz, izquierdo, derecho.
 */
typedef enum {
    E_INORDEN,   /**< Recorrido en inorden (izq-raiz-der). */
    E_POSTORDEN, /**< Recorrido en postorden (izq-der-raiz). */
    E_PREORDEN   /**< Recorrido en preorden (raiz-izq-der). */
} eTipoRecorrido;

/**
 * @struct tNodoArbol
 * @brief Nodo del arbol binario de busqueda.
 *
 * Cada nodo almacena una copia del dato y punteros a los subarboles
 * izquierdo y derecho.
 */
typedef struct sNodoArbol tNodoArbol;
struct sNodoArbol {
    tNodoArbol *izq, /**< Puntero al subarbol izquierdo. */
               *der; /**< Puntero al subarbol derecho. */
    unsigned n;      /**< Tamanio en bytes del dato almacenado. */
    void *data;      /**< Puntero al dato copiado. */
};

/**
 * @typedef tArbolBinBusq
 * @brief Puntero a la raiz del arbol binario de busqueda.
 *
 * Un valor NULL indica que el arbol esta vacio.
 */
typedef tNodoArbol *tArbolBinBusq;

/**
 * @brief Inicializa un arbol binario de busqueda vacio.
 *
 * Debe llamarse antes de cualquier otra operacion sobre el arbol.
 *
 * @param bst Puntero al arbol a inicializar.
 */
void arbolBinBusqCrear(tArbolBinBusq *bst);

/**
 * @brief Inserta una copia del dato en el arbol segun la funcion de comparacion.
 *
 * Si ya existe un dato equivalente (cmp retorna 0), no se inserta y
 * se retorna ERR. La insercion mantiene la propiedad de ABB.
 *
 * @param bst  Puntero al arbol.
 * @param data Puntero al dato origen que se copiara.
 * @param n    Tamanio en bytes del dato a copiar.
 * @param cmp  Funcion comparadora para determinar la posicion de insercion.
 * @return OK si la insercion tuvo exito, ERR si el dato ya existe o fallo la memoria.
 */
int arbolBinBusqPoner(
    tArbolBinBusq *bst,
    const void *data,
    unsigned n,
    fnCmp cmp
);

/**
 * @brief Extrae del arbol el dato que coincida con la clave proporcionada.
 *
 * Busca el nodo cuya data sea igual a \p clave segun \p cmp, lo elimina
 * del arbol y copia su contenido en \p buf.
 *
 * @param bst   Puntero al arbol.
 * @param clave Puntero al dato clave que se desea extraer.
 * @param buf   Puntero al buffer donde se copiara el dato extraido.
 * @param n     Tamanio en bytes esperado del dato.
 * @param cmp   Funcion comparadora.
 * @return OK si la extraccion tuvo exito, ERR si la clave no se encontro.
 */
int arbolBinBusqSacar(
    tArbolBinBusq *bst,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
);

/**
 * @brief Busca un dato en el arbol sin extraerlo.
 *
 * Recorre el arbol segun \p cmp hasta encontrar un nodo igual a \p clave
 * y copia su contenido en \p buf.
 *
 * @param bst   Puntero constante al arbol.
 * @param clave Puntero al dato clave que se desea buscar.
 * @param buf   Puntero al buffer donde se copiara el dato encontrado.
 * @param n     Tamanio en bytes esperado del dato.
 * @param cmp   Funcion comparadora.
 * @return OK si se encontro el dato, ERR si no existe en el arbol.
 */
int arbolBinBusqBuscar(
    const tArbolBinBusq *bst,
    const void *clave,
    void *buf,
    unsigned n,
    fnCmp cmp
);

/**
 * @brief Imprime todos los nodos del arbol segun el recorrido indicado.
 *
 * Recorre el arbol en el orden especificado e invoca la funcion
 * \p imprimir sobre el dato de cada nodo.
 *
 * @param bst       Puntero constante al arbol.
 * @param tipo      Tipo de recorrido a realizar.
 * @param imprimir  Funcion de impresion (fnImprimir).
 */
void arbolBinBusqImprimir(
    const tArbolBinBusq *bst,
    eTipoRecorrido tipo,
    fnImprimir imprimir
);

/**
 * @brief Recorre el arbol aplicando una accion sobre cada nodo.
 *
 * A diferencia de arbolBinBusqImprimir, esta funcion permite pasar
 * un contexto adicional (\p usuario) a la funcion de accion,
 * habilitando operaciones como acumuladores o modificaciones
 * sobre los datos almacenados.
 *
 * @param bst      Puntero al arbol.
 * @param tipo     Tipo de recorrido a realizar.
 * @param accion   Funcion que se ejecutara sobre cada nodo.
 * @param usuario Puntero con contexto adicional pasado a la accion.
 */
void arbolBinBusqVisitar(
    tArbolBinBusq *bst,
    eTipoRecorrido tipo,
    fnAccion accion,
    void *usuario
);

/**
 * @brief Destruye el arbol y libera toda la memoria asociada.
 *
 * Elimina todos los nodos y sus datos, dejando el arbol en estado vacio.
 * Despues de llamar esta funcion se puede volver a usar arbolBinBusqCrear().
 *
 * @param bst Puntero al arbol a destruir.
 */
void arbolBinBusqDestruir(tArbolBinBusq *bst);

/**
 * @brief Cuenta la cantidad total de nodos del arbol.
 *
 * @param a Puntero constante al arbol.
 * @return Cantidad de nodos, 0 si el arbol esta vacio.
 */
int arbolBinBusqContarNodos(const tArbolBinBusq *a);

#endif
