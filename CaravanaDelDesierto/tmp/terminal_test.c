#include <stdio.h>
#include "terminal.h"

int main(void) {
    tTerminal term;
    unsigned x, y;
    int shouldExit = 0;
    tEventoTecla evento;
    if (terminalCrear(&term)) return 1;
    if (terminalEntrarModoCrudo(&term)) return 2;
    terminalObtenerTam(&term, &x, &y);
    printf("rows=%u cols=%u\n", x, y);
    char buf[256];
    char bufPos = 0;

    while(!shouldExit) {
        terminalLeerEvento(&term, &evento);

        if(evento.tipo == TECLA_ESC) {
            shouldExit = 1;
            continue;
        }

        if(evento.tipo == TECLA_CARACTER) {
            buf[bufPos++] = evento.caracter;
        } else {
            switch(evento.tipo) {
                case TECLA_ABAJO:
                    terminalEscribir(&term, "Abajo\n");
                    break;
                case TECLA_ARRIBA:
                    terminalEscribir(&term, "Arriba\n");
                    break;
                case TECLA_IZQUIERDA:
                    terminalEscribir(&term, "Izquierda\n");
                    break;
                case TECLA_DERECHA:
                    terminalEscribir(&term, "Derecha\n");
                    break;
                default:
                    break;
            }
        }

        terminalActualizar(&term);
    }

    terminalSalirModoCrudo(&term);
    terminalDestruir(&term);
    printf("Exito\n");
    return 0;
}
