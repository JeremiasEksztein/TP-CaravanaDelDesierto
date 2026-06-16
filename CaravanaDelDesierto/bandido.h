/**
 * @file bandido.h
 * @brief Operaciones para la creacion, consulta, movimiento y destruccion de
 *        bandidos en el juego "Caravana del Desierto".
 *
 * Los bandidos son enemigos que se desplazan por el tablero. Al coincidir
 * con la posicion del jugador le quitan una vida. Este modulo expone las
 * primitivas necesarias para gestionar su ciclo de vida completo.
 */

#ifndef BANDIDO_H
#define BANDIDO_H

/*Tipos de bandido*/
typedef struct {
  int id;
  int pos;
  int posAnterior;
  int mov;
} tBandido;
/**
 * @brief Inicializa un bandido con valores por defecto.
 *
 * Asigna la posicion inicial en 0, genera un identificador unico aleatorio
 * y establece la posicion anterior en 0.
 *
 * @param bandido Puntero a la estructura tBandido a inicializar.
 */
void crearBandido(tBandido *bandido);

/**
 * @brief Obtiene el identificador unico del bandido.
 *
 * @param bandido Puntero constante a la estructura tBandido.
 * @return El ID del bandido.
 */
int obtenerIdBandido(const tBandido *bandido);

/**
 * @brief Obtiene la posicion actual del bandido en el tablero.
 *
 * @param bandido Puntero a la estructura tBandido.
 * @return La posicion actual (indice de casilla).
 */
int obtenerPosBandido(tBandido *bandido);

/**
 * @brief Obtiene la posicion anterior del bandido en el tablero.
 *
 * Util para restaurar el estado de una casilla luego de que el bandido
 * se haya movido.
 *
 * @param bandido Puntero a la estructura tBandido.
 * @return La posicion anterior (indice de casilla).
 */
int obtenerPosAnteriorBandido(tBandido *bandido);

/**
 * @brief Actualiza la posicion del bandido a un nuevo valor generico.
 *
 * @param bandido Puntero a la estructura tBandido.
 * @param pos    Puntero generico con la nueva posicion (tipicamente un int).
 */
void actualizarPosBandido(tBandido *bandido, void *pos);

/**
 * @brief Libera los recursos asociados a un bandido.
 *
 * @param bandido Puntero a la estructura tBandido a destruir.
 */
void destruirBandido(tBandido *bandido);

/**
 * @brief Coloca al bandido en una posicion especifica del tablero.
 *
 * Asigna directamente la posicion sin conservar el valor anterior.
 *
 * @param b   Puntero a la estructura tBandido.
 * @param pos Indice de la casilla donde se ubicara el bandido.
 */
void colocarBandidoEnPos(tBandido *b, const int pos);

/**
 * @brief Desplaza al bandido sobre el tablero con envoltura circular.
 *
 * Calcula la nueva posicion como <tt>pos + mov</tt> y aplica
 * aritmetica modular para que el bandido "rebote" dentro de los limites
 * del tablero. Ademas registra la posicion anterior y el movimiento
 * realizado.
 *
 * @param b   Puntero a la estructura tBandido.
 * @param mov Cantidad de casillas a moverse (negativo = izquierda).
 * @param n   Cantidad total de casillas del tablero (modulo).
 */
void moverBandido(tBandido *b, const int mov, const int n);

int BuscarIndiceDeBandido(const tBandido* arrayBandidos, const int id, int cantBandidos);


#endif
