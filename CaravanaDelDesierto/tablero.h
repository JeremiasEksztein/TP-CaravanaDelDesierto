#ifndef TABLERO_H
#define TABLERO_H

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

#endif
