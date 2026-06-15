/**
 * @file jugador.h
 * @brief Funciones para la creacion, consulta y modificacion del estado del
 *        jugador en el juego "Caravana del Desierto".
 *
 * Este modulo define las operaciones sobre la entidad tJugador: inicializacion,
 * movimiento, gestion de vidas, puntos, invulnerabilidad y control de turnos.
 */

#ifndef JUGADOR_H
#define JUGADOR_H

/*Tipos de jugador*/
#define TAM_NOMBRE 10
typedef struct {
  int vidas;
  char name[TAM_NOMBRE];
  int puntos;
  int invulnerable;
  int omitirTurno;
  int posAnterior;
  int pos;
  int mov;
} tJugador;

/**
 * @brief Inicializa un jugador con un nombre y cantidad de vidas.
 *
 * Establece los valores por defecto: 0 puntos, invulnerable activado,
 * sin turno omitido, posiciones en 0 y sin movimiento previo.
 *
 * @param j     Puntero al jugador a inicializar.
 * @param name  Nombre del jugador (maximo TAM_NOMBRE - 1 caracteres).
 * @param vidas Cantidad inicial de vidas del jugador.
 */
void crearJugador(tJugador *j, const char *name, const int vidas);

/**
 * @brief Obtiene la posicion actual del jugador en el tablero.
 *
 * @param j Puntero constante al jugador.
 * @return Indice de la casilla donde se encuentra el jugador.
 */
int obtenerPosJugador(const tJugador *j);

/**
 * @brief Obtiene la posicion anterior del jugador en el tablero.
 *
 * Util para detectar la direccion del ultimo desplazamiento o restaurar
 * el estado previo al movimiento.
 *
 * @param j Puntero constante al jugador.
 * @return Indice de la casilla donde estaba el jugador antes del ultimo
 * movimiento.
 */
int obtenerPosAnteriorJugador(const tJugador *j);

/**
 * @brief Resta una vida al jugador.
 *
 * Decrementa en 1 el contador de vidas. Si las vidas llegan a 0,
 * corresponde al llamante verificar la condicion de fin de juego.
 *
 * @param j Puntero al jugador afectado.
 */
void quitarVida(tJugador *j);

/**
 * @brief Incrementa en 1 los puntos del jugador.
 *
 * Se invoca cuando el jugador cae en una casilla de premio.
 *
 * @param j Puntero al jugador que recibe el punto.
 */
void obtenerPunto(tJugador *j);

/**
 * @brief Restablece la bandera de omitir turno.
 *
 * Despues de que el jugador haya perdido un turno, esta funcion
 * lo habilita nuevamente para el siguiente.
 *
 * @param j Puntero al jugador.
 */
void quitarOmitirTurno(tJugador *j);

/**
 * @brief Marca al jugador para que pierda el proximo turno.
 *
 * Activa la bandera omitirTurno. El jugador no podra moverse en la
 * siguiente ronda hasta que se llame a quitarOmitirTurno().
 *
 * @param j Puntero al jugador afectado.
 */
void omitirTurno(tJugador *j);

/**
 * @brief Desactiva la invulnerabilidad del jugador.
 *
 * Una vez desactivada, el jugador es vulnerable a los bandidos
 * y perdera una vida si coinciden en la misma casilla.
 *
 * @param j Puntero al jugador.
 */
void quitarInvulnerable(tJugador *j);

/**
 * @brief Activa la invulnerabilidad del jugador.
 *
 * Mientras este activa, los bandidos no pueden quitarle vidas al jugador
 * aunque compartan la misma casilla. Se desactiva automaticamente al
 * realizar un movimiento.
 *
 * @param j Puntero al jugador.
 */
void hacerInvulnerable(tJugador *j);

/**
 * @brief Coloca al jugador en una posicion especifica del tablero.
 *
 * Guarda la posicion actual como posAnterior antes de reasignar.
 * Util para teletransportes o colocacion inicial.
 *
 * @param j   Puntero al jugador.
 * @param pos Indice de la casilla destino.
 */
void colocarJugadorEnPos(tJugador *j, const int pos);

/**
 * @brief Desplaza al jugador sobre el tablero con rebote en los extremos.
 *
 * El movimiento se calcula como pos + mov. El tablero se comporta
 * como un recorrido con reflejo en los bordes: cuando el desplazamiento
 * excede el rango [0, n-1], rebota en sentido contrario.
 *
 * Al moverse, el jugador pierde automaticamente la invulnerabilidad.
 *
 * @param j   Puntero al jugador.
 * @param mov Cantidad de casillas a desplazarse (negativo = izquierda).
 * @param n   Cantidad total de casillas del tablero.
 */
void moverJugador(tJugador *j, const int mov, const int n);

/**
 * @brief Incrementa en una las vidas del jugador.
 *
 * Se invoca cuando el jugador cae en una casilla de vida extra.
 *
 * @param j Puntero al jugador que recibe la vida.
 */
void obtenerVida(tJugador *j);

#endif
