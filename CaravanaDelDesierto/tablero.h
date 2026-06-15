#ifndef TABLERO_H
#define TABLERO_H


#include <string.h>

#define FILE_TABLERO "caravana.txt"
#define CASILLA_NORMAL '.'
#define CASILLA_OASIS 'O'
#define CASILLA_TORMENTA 'T'
#define CASILLA_PREMIO 'P'
#define CASILLA_VIDA 'V'
#define CASILLA_BANDIDO 'B'
#define CASILLA_META 'S'
#define CASILLA_INICIO 'I'
#define CASILLA_JUGADOR 'J'
#define OK 0
//Otros defines
#define ARCHIVO_CONFIG_TABLERO "config.txt"
#define ERROR_ARCHIVO 4
#define TAM_LINEA 150
#define ES_PRIMERA_LINEA cfg->cantCasillas==1

//Sencilla configuracion por defecto, para testing:
#define CANT_CASILLAS_DEFAULT 20
#define CANT_BANDIDOS_DEFAULT 3
#define CANT_OASIS_DEFAULT 2
#define CANT_PREMIO_DEFAULT 2
#define CANT_VIDA_DEFAULT 2
#define CANT_TOR_DEFAULT 2


typedef unsigned char tTipoCasilla; // 1 byte

typedef struct {
  tTipoCasilla base;        // O/T/P/V/I/S/.
  unsigned char hayJugador; // 0/1
  unsigned char hayBandido; // 0/1
} tCasilla;

typedef struct {
  double_list_t casillas;
  int cant;
} tTablero;

typedef struct {
  int cantCasillas;
  int maxBands;
  int maxPrem;
  int maxVida;
  int maxOasis;
  int maxTor;
} tConfigTablero;

void crearTablero(tTablero *t, int nCasillas);
int distribuirCasillas(tTablero *t, const tConfigTablero *cfg, tJugador *j,
                       tBandido *b);
void moverPiezaDeTablero(tTipoCasilla tc, void *pieza, tTablero *t);
int verificarCapacidad(const tConfigTablero *cfg);
int buscarJugadorEnTablero(tTablero *t);
void mostrarTablero(const tTablero *t);
void eliminarBandidoDeTablero(tTablero *t, tBandido *b);


//OTRAS FUNCIONES AUXILIARES PARA EL MANEJO DEL TABLERO:
int CargarConfiguracionDeTablero(const char *nombreArchivo, tConfigTablero *cfg);
int ContarOcurrencias(const char* cadena, char caracter) ;
int CargarConfiguracionPorDefecto(tConfigTablero *cfg);
#endif
