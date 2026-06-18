#ifndef TABLERO_H
#define TABLERO_H

#include "bandido.h"
#include "jugador.h"
#include "listaCircularDoble.h"

#include <string.h>

/**
 * @def FILE_TABLERO
 * @brief Nombre del archivo que contiene la configuración del tablero.
 */
#define FILE_TABLERO "caravana.txt"

/**
 * @def CASILLA_NORMAL
 * @brief Carácter que representa una casilla normal sin efecto especial.
 */
#define CASILLA_NORMAL '.'

/**
 * @def CASILLA_OASIS
 * @brief Carácter que representa una casilla de tipo oasis.
 */
#define CASILLA_OASIS 'O'

/**
 * @def CASILLA_TORMENTA
 * @brief Carácter que representa una casilla de tipo tormenta.
 */
#define CASILLA_TORMENTA 'T'

/**
 * @def CASILLA_PREMIO
 * @brief Carácter que representa una casilla con premio.
 */
#define CASILLA_PREMIO 'P'

/**
 * @def CASILLA_VIDA
 * @brief Carácter que representa una casilla que otorga una vida extra.
 */
#define CASILLA_VIDA 'V'

/**
 * @def CASILLA_BANDIDO
 * @brief Carácter que representa una casilla ocupada por un bandido.
 */
#define CASILLA_BANDIDO 'B'

/**
 * @def CASILLA_META
 * @brief Carácter que representa la casilla de meta del tablero.
 */
#define CASILLA_META 'S'

/**
 * @def CASILLA_INICIO
 * @brief Carácter que representa la casilla de inicio del tablero.
 */
#define CASILLA_INICIO 'I'

/**
 * @def CASILLA_JUGADOR
 * @brief Carácter que representa la posición actual del jugador.
 */
#define CASILLA_JUGADOR 'J'
#define OK 0
/* Otros defines */

/**
 * @def ERROR_ARCHIVO
 * @brief Código de error devuelto cuando falla la apertura de un archivo.
 */
#define ERROR_ARCHIVO 4

/**
 * @def TAM_LINEA
 * @brief Tamaño máximo de una línea leída del archivo de configuración.
 */
#define TAM_LINEA 150

/**
 * @def ES_PRIMERA_LINEA
 * @brief Macro que evalúa si la línea actual es la primera del archivo de configuración.
 */
#define ES_PRIMERA_LINEA cfg->cantCasillas == 1

/**
 * @typedef tTipoCasilla
 * @brief Tipo numerico para identificar el tipo de casilla.
 */
typedef unsigned char tTipoCasilla;

/**
 * @typedef tTipoPieza
 * @brief Tipo numérico para identificar la pieza que ocupa una casilla
 *        (jugador, bandido o vacía).
 */
typedef unsigned char tTipoPieza;

/**
 * @struct tCasilla
 * @brief Representa una casilla individual del tablero.
 *
 * Cada casilla tiene un tipo base (normal, oasis, tormenta, etc.),
 * una pieza que la ocupa (jugador, bandido o vacio) y un contador
 * de bandidos presentes.
 */
typedef struct {
	tTipoCasilla base; /**< Tipo base de la casilla (CASILLA_NORMAL,
												CASILLA_OASIS, etc.) */
	tTipoPieza pieza;  /**< Pieza que ocupa la casilla ('J', 'B' o 0 si vacia) */
	int cantBandidosEnCasilla; /**< Cantidad de bandidos actualmente en esta
																casilla */
} tCasilla;

/**
 * @struct tTablero
 * @brief Representa el tablero circular del juego.
 *
 * Almacena las casillas en una lista circular doblemente enlazada
 * y la cantidad total de casillas que lo componen.
 */
typedef struct {
	tListaCircularDoble casillas; /**< Lista circular de casillas del tablero. */
	int cant; /**< Cantidad total de casillas del tablero. */
} tTablero;

/**
 * @struct tConfigTablero
 * @brief Configuración de parámetros para la generación del tablero.
 *
 * Define las cantidades máximas de cada tipo de casilla especial
 * y las vidas iniciales del jugador.
 */
typedef struct {
	int cantCasillas; /**< Cantidad total de casillas del tablero. */
	int maxBands;     /**< Cantidad máxima de bandidos en el tablero. */
	int maxPrem;      /**< Cantidad máxima de casillas con premio. */
	int maxVida;      /**< Cantidad máxima de casillas con vida extra. */
	int maxOasis;     /**< Cantidad máxima de casillas de tipo oasis. */
	int maxTor;       /**< Cantidad máxima de casillas de tipo tormenta. */
	int vidasInicio;  /**< Cantidad de vidas iniciales del jugador. */
} tConfigTablero;

/**
 * @brief Crea e inicializa un tablero con la cantidad de casillas indicada.
 *
 * Reserva la memoria necesaria para la lista circular de casillas y
 * las inicializa como casillas normales vacías.
 *
 * @param t Puntero al tablero a crear.
 * @param nCasillas Cantidad de casillas que tendrá el tablero.
 */
void crearTablero(tTablero* t, int nCasillas);

/**
 * @brief Distribuye las casillas especiales y posiciona al jugador y bandidos.
 *
 * Asigna los tipos de casilla (oasis, tormenta, premio, vida, bandido, inicio,
 * meta) de forma aleatoria y coloca al jugador y a los bandidos en sus
 * posiciones iniciales.
 *
 * @param t Puntero al tablero ya creado.
 * @param cfg Puntero a la configuración con las cantidades de cada tipo.
 * @param j Puntero al jugador a posicionar.
 * @param b Puntero al array de bandidos a posicionar.
 * @return 1 si la distribución fue exitosa, 0 en caso contrario.
 */
int distribuirCasillas(tTablero* t, const tConfigTablero* cfg, tJugador* j,
											 tBandido* b);

/**
 * @brief Mueve una pieza (jugador o bandido) dentro del tablero.
 *
 * Actualiza la posición de la pieza en la estructura correspondiente
 * y sincroniza su representación en el tablero.
 *
 * @param tc Tipo de casilla que identifica el tipo de pieza a mover.
 * @param pieza Puntero a la pieza (jugador o bandido) a mover.
 * @param t Puntero al tablero donde se realiza el movimiento.
 */
void moverPiezaDeTablero(tTipoCasilla tc, void* pieza, tTablero* t);

/**
 * @brief Verifica que la configuración del tablero sea coherente.
 *
 * Comprueba que la suma de casillas especiales no exceda la
 * cantidad total de casillas del tablero.
 *
 * @param cfg Puntero a la configuración del tablero a verificar.
 * @return 1 si la configuración es válida, 0 en caso contrario.
 */
int verificarCapacidad(const tConfigTablero* cfg);

/**
 * @brief Busca la posición del jugador en el tablero.
 *
 * Recorre las casillas del tablero hasta encontrar la que contiene
 * la pieza del jugador.
 *
 * @param t Puntero al tablero donde buscar.
 * @return Índice de la casilla donde se encuentra el jugador, o -1 si no se encuentra.
 */
int buscarJugadorEnTablero(tTablero* t);

/**
 * @brief Muestra el tablero completo en pantalla con formato detallado.
 *
 * Imprime cada casilla con su tipo, pieza y información de bandidos.
 *
 * @param t Puntero constante al tablero a mostrar.
 */
void mostrarTablero(const tTablero* t);

/**
 * @brief Muestra una versión compacta del tablero centrada en la posición del jugador.
 *
 * Imprime solo las casillas visibles alrededor del jugador, útil para
 * la vista de juego en tiempo real.
 *
 * @param t Puntero constante al tablero a mostrar.
 * @param posJugador Posición actual del jugador en el tablero.
 */
void mostrarTableroCompacto(const tTablero* t, int posJugador);

/**
 * @brief Guarda el tablero en un archivo de texto, en el mismo formato
 *        que mostrarTableroCompacto.
 *
 * Escribe cada casilla como [base J/B/.] al archivo indicado.
 * Si el archivo no se puede abrir, retorna silenciosamente sin
 * interrumpir el juego.
 *
 * @param t Puntero constante al tablero a guardar.
 * @param posJugador Posicion actual del jugador en el tablero.
 * @param nombreArchivo Ruta del archivo donde se escribira el tablero.
 */
void guardarTablero(const tTablero* t, int posJugador, const char* nombreArchivo);

/**
 * @brief Limpia la pantalla de la terminal.
 *
 * Emite la secuencia de escape ANSI para borrar el contenido visible
 * de la terminal y reposicionar el cursor.
 */
void limpiarPantalla(void);

/**
 * @brief Pausa la ejecución durante la cantidad de milisegundos indicada.
 *
 * Utiliza funciones del sistema para suspender el hilo actual.
 *
 * @param ms Cantidad de milisegundos a esperar.
 */
void esperar(unsigned int ms);

/**
 * @brief Elimina un bandido del tablero, quitándolo de su casilla actual.
 *
 * Busca la casilla del bandido, decrementa el contador de bandidos
 * y actualiza la pieza de la casilla si corresponde.
 *
 * @param t Puntero al tablero donde se encuentra el bandido.
 * @param b Puntero al bandido a eliminar del tablero.
 */
void eliminarBandidoDeTablero(tTablero* t, tBandido* b);
/**
 * @brief Decrementa en uno el contador de bandidos de una casilla.
 *
 * Si la cantidad de bandidos llega a cero, elimina la pieza de la casilla.
 * Util para reflejar la eliminacion o salida de un bandido de una posicion.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla de la que se quita el bandido.
 */
void quitarBandidoDePos(tTablero* t, int pos);

/**
 * @brief Quita al jugador de una casilla especifica del tablero.
 *
 * Elimina la pieza del jugador ('J') de la casilla indicada, dejandola
 * con su tipo base original.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla de la que se quita al jugador.
 */
void quitarJugadorDePos(tTablero* t, int pos);

/**
 * @brief Coloca al jugador en una casilla especifica del tablero.
 *
 * Asigna la pieza del jugador ('J') a la casilla indicada.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla donde se coloca al jugador.
 */
void ponerJugadorEnPosTablero(tTablero* t, int pos);

/**
 * @brief Consume el efecto especial de una casilla, devolviendola a estado
 * normal.
 *
 * Transforma el tipo base de la casilla en la posicion indicada a
 * CASILLA_NORMAL. Se utiliza despues de que el jugador haya obtenido
 * un premio o una vida extra en esa casilla.
 *
 * @param t   Puntero al tablero donde se encuentra la casilla.
 * @param pos Indice de la casilla a consumir.
 */
void consumirCasilla(tTablero* t, int pos);

/**
 * @brief Sincroniza la posicion del jugador en el tablero.
 *
 * Quita al jugador de su posicion anterior y lo coloca en la actual.
 * Maneja correctamente casillas con bandidos.
 *
 * @param j Puntero al jugador a sincronizar.
 * @param t Puntero al tablero donde se realiza el movimiento.
 */
void sincronizarJugadorEnTablero(tJugador* j, tTablero* t);

/**
 * @brief Sincroniza la posicion de un bandido en el tablero.
 *
 * Quita el bandido de su posicion anterior (decrementando contador) y
 * lo coloca en la nueva (incrementando contador).
 *
 * @param b Puntero al bandido a sincronizar.
 * @param t Puntero al tablero donde se realiza el movimiento.
 */
void sincronizarBandidoEnTablero(tBandido* b, tTablero* t);

/* OTRAS FUNCIONES AUXILIARES PARA EL MANEJO DEL TABLERO: */

/**
 * @brief Carga la configuración del tablero desde un archivo de texto.
 *
 * Lee el archivo especificado y parsea los parámetros de configuración
 * (cantidad de casillas, bandidos, premios, etc.) en la estructura dada.
 *
 * @param nombreArchivo Ruta al archivo de configuración.
 * @param cfg Puntero a la estructura donde se almacenará la configuración.
 * @return 1 si la carga fue exitosa, 0 en caso de error.
 */
int CargarConfiguracionDeTablero(const char* nombreArchivo,
																 tConfigTablero* cfg);

/**
 * @brief Cuenta la cantidad de ocurrencias de un carácter en una cadena.
 *
 * Recorre la cadena y devuelve cuántas veces aparece el carácter dado.
 *
 * @param cadena Cadena de caracteres a analizar.
 * @param caracter Carácter cuya ocurrencia se desea contar.
 * @return Cantidad de veces que aparece el carácter en la cadena.
 */
int ContarOcurrencias(const char* cadena, char caracter);

/**
 * @brief Carga una configuración por defecto del tablero.
 *
 * Asigna valores predeterminados a todos los campos de la estructura
 * de configuración, útiles para testing o cuando no hay archivo disponible.
 *
 * @param cfg Puntero a la estructura donde se almacenará la configuración por defecto.
 * @return 1 si la carga fue exitosa, 0 en caso de error.
 */
int CargarConfiguracionPorDefecto(tConfigTablero* cfg);
#endif
