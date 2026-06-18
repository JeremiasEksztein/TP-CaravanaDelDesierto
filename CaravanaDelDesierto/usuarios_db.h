/**
 * @file usuarios_db.h
 * @brief Sistema de persistencia ISAM con indice en arbol binario de busqueda.
 *
 *  Este modulo implementa una tabla indexada de registros en archivos
 *  binarios, similar a un esquema ISAM (Indexed Sequential Access Method).
 *  El archivo de datos almacena los registros de forma secuencial, mientras
 *  que un arbol binario de busqueda en memoria actua como indice para
 *  permitir busquedas, actualizaciones y recorridos eficientes.
 *
 *  Se proveen tipos y funciones especificas para la entidad "Jugador",
 *  incluyendo comparacion por nombre y extraccion de clave, aunque la
 *  estructura tTabla es generica y puede adaptarse a otros registros.
 */

#ifndef USUARIOS_DB_H_INCLUDED
#define USUARIOS_DB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbolBinBusq.h"
#include "common.h"

/** @def TAM_NOMBRE_JUG
 *  @brief Longitud maxima del nombre de un jugador (incluye terminador nulo). */
#define TAM_NOMBRE_JUG 11

/** @def JUGADORES_DB
 *  @brief Nombre del archivo binario de datos de jugadores. */
#define JUGADORES_DB "jugadores.dat"

/** @def JUGADORES_IDX
 *  @brief Nombre del archivo binario de indice de jugadores. */
#define JUGADORES_IDX "jugadores.idx"

/** @def PARTIDA_DB
 *  @brief Nombre del archivo binario de datos de partidas. */
#define PARTIDA_DB "partidas.dat"

/** @def ARCHPATHLEN
 *  @brief Longitud maxima para las rutas de archivo dentro de tTabla. */
#define ARCHPATHLEN 256

/** @def INDICE_CLAVE_LEN
 *  @brief Longitud maxima de la clave de busqueda en una entrada de indice. */
#define INDICE_CLAVE_LEN 256

/**
 * @typedef fnLeerClave
 * @brief Puntero a funcion que extrae la clave de busqueda de un registro.
 *
 *  Recibe un buffer de salida y un registro de entrada; copia la clave
 *  de busqueda del registro al buffer de salida. Se utiliza para insertar
 *  y buscar entradas en el indice.
 *
 * @param out Buffer donde se almacenara la clave extraida.
 * @param in  Puntero al registro del cual se extrae la clave.
 * @return OK si la extraccion fue exitosa, ERR en caso contrario.
 */
typedef int (*fnLeerClave)(void *, const void *);

/**
 * @struct tRegistroJugador
 * @brief Registro persistente de un jugador en el archivo de datos.
 *
 *  Almacena el identificador, nombre y cantidad de partidas jugadas
 *  de un jugador. Se graba directamente en formato binario.
 */
typedef struct {
  /** @brief Identificador unico del jugador. */
  int id;
  /** @brief Nombre del jugador (maximo TAM_NOMBRE_JUG - 1 caracteres). */
  char nombre[TAM_NOMBRE_JUG];
  /** @brief Cantidad total de partidas jugadas por este jugador. */
  int partidasJugadas;
} tRegistroJugador;

/**
 * @struct tRegistroPartida
 * @brief Registro persistente de una partida en el archivo de datos.
 *
 *  Almacena los resultados de una partida: que jugador la jugo, cuantos
 *  puntos obtuvo y cuantos movimientos realizo.
 */
typedef struct {
  /** @brief Identificador del jugador que disputo la partida. */
  int idJugador;
  /** @brief Puntos obtenidos durante la partida. */
  int puntosObtenidos;
  /** @brief Cantidad de movimientos realizados en la partida. */
  int movimientosRealizados;
} tRegistroPartida;

/**
 * @struct tTabla
 * @brief Tabla indexada ISAM con archivo de datos y arbol binario como indice.
 *
 *  Combina un archivo binario de acceso aleatorio para los datos con un
 *  arbol binario de busqueda en memoria que actua como indice. Las
 *  escrituras son append-only (agregar al final) y las actualizaciones
 *  se realizan in-place por desplazamiento (offset).
 */
typedef struct {
  /** @brief Puntero al archivo de datos abierto en modo "rb+". */
  FILE *archDatos;

  /** @brief Arbol binario de busqueda que contiene las entradas del indice. */
  tArbolBinBusq indice;
  /** @brief Ruta del archivo de datos. */
  char pathDatos[ARCHPATHLEN];
  /** @brief Ruta del archivo de indice. */
  char pathIndice[ARCHPATHLEN];
  /** @brief Tamaño en bytes de cada registro en el archivo de datos. */
  unsigned regTam;
  /** @brief Tamaño en bytes de la clave de busqueda. */
  unsigned claveTam;

  /** @brief Funcion de comparacion para ordenar y buscar en el indice. */
  fnCmp cmp;
  /** @brief Funcion para extraer la clave de un registro. */
  fnLeerClave leerClave;

  /** @brief Bandera que indica si el indice fue modificado y necesita ser
   *         vuelto a grabar al cerrar. */
  int indiceSucio;
} tTabla;

/**
 * @struct tEntradaIndice
 * @brief Entrada individual del indice de la tabla.
 *
 *  Cada nodo del arbol binario de busqueda almacena una entrada con la
 *  clave de busqueda, el offset del registro en el archivo de datos y
 *  el identificador del registro.
 */
typedef struct {
  /** @brief Clave de busqueda (ej. nombre del jugador). */
  char clave[INDICE_CLAVE_LEN];
  /** @brief Desplazamiento (en registros) del dato dentro del archivo. */
  long offset;
  /** @brief Identificador unico del registro. */
  int id;
} tEntradaIndice;

/**
 * @brief Inicializa una tabla indexada con sus rutas y funciones callback.
 *
 *  Configura la estructura tTabla con las rutas de los archivos de datos
 *  e indice, las funciones de comparacion y extraccion de clave, y los
 *  tamanios de registro y clave. No abre los archivos; para ello usar
 *  tablaAbrir().
 *
 * @param t         Puntero a la tabla a inicializar.
 * @param pathDatos Ruta del archivo binario de datos.
 * @param pathIndice Ruta del archivo binario de indice.
 * @param cmp       Funcion de comparacion para el arbol indice.
 * @param leer      Funcion para extraer la clave de un registro.
 * @param regTam    Tamaño en bytes de cada registro de datos.
 * @param claveTam  Tamaño en bytes de la clave de busqueda.
 * @return OK si la inicializacion fue exitosa, ERR si los parametros son invalidos.
 */
int tablaCrear(tTabla *t, const char *pathDatos, const char *pathIndice,
               fnCmp cmp, fnLeerClave leer, unsigned regTam, unsigned claveTam);

/**
 * @brief Compara dos entradas de indice por clave (nombre de jugador).
 *
 *  Funcion de comparacion especifica para el indice de jugadores.
 *  Utiliza strcmp sobre el campo clave de cada entrada.
 *
 * @param a Puntero a la primera entrada de indice (castear a tEntradaIndice*).
 * @param b Puntero a la segunda entrada de indice (castear a tEntradaIndice*).
 * @return Negativo, cero o positivo segun la relacion de orden lexicografico.
 */
int jugadorIndiceCmp(const void *a, const void *b);

/**
 * @brief Extrae la clave (nombre) de un registro de jugador.
 *
 *  Copia el campo nombre del registro de entrada al buffer de salida,
 *  garantizando la terminacion nula. Se utiliza como callback fnLeerClave
 *  para la tabla de jugadores.
 *
 * @param out Buffer de salida donde se almacenara la clave extraida.
 * @param in  Puntero al registro tRegistroJugador del cual se extrae el nombre.
 * @return OK si la extraccion fue exitosa.
 */
int jugadorLeerClave(void *out, const void *in);

/**
 * @brief Abre los archivos de datos e indice de la tabla.
 *
 *  Abre el archivo de datos en modo lectura/escritura (lo crea si no
 *  existe). Carga el indice desde el archivo de indice; si este no
 *  existe, reconstruye el indice recorriendo todo el archivo de datos.
 *
 * @param t Puntero a la tabla previamente inicializada con tablaCrear().
 * @return OK si la apertura fue exitosa, ERR si no se pudieron abrir los archivos.
 */
int tablaAbrir(tTabla *t);

/**
 * @brief Agrega un nuevo registro al final del archivo de datos y lo indexa.
 *
 *  Escribe el registro en el archivo de datos (append), inserta la
 *  entrada correspondiente en el arbol indice y guarda el indice en
 *  disco. Si la clave ya existe en el indice, retorna ERR.
 *
 * @param t   Puntero a la tabla abierta.
 * @param reg Puntero al registro a agregar (de tamaño regTam bytes).
 * @return OK si el alta fue exitosa, ERR si la clave ya existe o hubo error.
 */
int tablaIngresar(tTabla *t, const void *reg);

/**
 * @brief Busca un registro por su clave y lo copia al buffer proporcionado.
 *
 *  Utiliza el arbol indice para localizar el offset del registro y luego
 *  lo lee directamente del archivo de datos.
 *
 * @param t     Puntero a la tabla abierta.
 * @param clave Puntero al registro que contiene la clave de busqueda.
 * @param buf   Buffer donde se copiara el registro encontrado.
 * @return OK si el registro fue encontrado, ERR si no existe o hubo error.
 */
int tablaBuscar(tTabla *t, const void *clave, void *buf);

/**
 * @brief Actualiza un registro existente identificado por su clave.
 *
 *  Busca el registro en el indice y lo sobreescribe in-place en el
 *  archivo de datos usando el offset almacenado en la entrada del indice.
 *
 * @param t     Puntero a la tabla abierta.
 * @param clave Puntero al registro que contiene la clave de busqueda.
 * @param reg   Puntero al registro con los datos actualizados.
 * @return OK si la actualizacion fue exitosa, ERR si la clave no se encontro.
 */
int tablaActualizar(tTabla *t, const void *clave, const void *reg);

/**
 * @brief Recorre todos los registros del archivo de datos aplicando una funcion.
 *
 *  Lee secuencialmente cada registro del archivo de datos y ejecuta la
 *  funcion de callback proporcionada sobre el.
 *
 * @param t       Puntero a la tabla abierta.
 * @param accion Funcion de callback que recibe cada registro leido.
 * @param usuario Puntero opcional pasado como contexto adicional a la funcion de callback.
 * @return OK si el recorrido se completo, ERR si los parametros son invalidos.
 */
int tablaRecorrer(tTabla *t, fnAccion accion, void *usuario);

/**
 * @brief Cierra los archivos y guarda el indice si fue modificado.
 *
 *  Vacía el buffer del archivo de datos, lo cierra y, si el indice
 *  fue marcado como sucio (modificado), lo graba en disco en preorden.
 *
 * @param t Puntero a la tabla abierta.
 * @return OK si el cierre fue exitoso, ERR si hubo errores de escritura.
 */
int tablaCerrar(tTabla *t);

/**
 * @brief Libera todos los recursos asociados a la tabla.
 *
 *  Cierra el archivo de datos si esta abierto y destruye el arbol
 *  indice en memoria. La tabla queda en estado invalido y no debe
 *  usarse sin reinicializarla con tablaCrear().
 *
 * @param t Puntero a la tabla a destruir.
 */
void tablaDestruir(tTabla *t);

/**
 * @brief Calcula el prossimo identificador disponible basandose en el tamaño del archivo.
 *
 *  Determina cuantos registros contiene el archivo de datos dividiendo
 *  su tamaño total por el tamaño de un registro. El resultado puede
 *  utilizarse como ID para el proximo registro a insertar.
 *
 * @param t Puntero a la tabla abierta.
 * @return El numero de registros existentes (proximo ID), o 0 si hay error.
 */
int tablaProximoId(tTabla *t);


#endif
