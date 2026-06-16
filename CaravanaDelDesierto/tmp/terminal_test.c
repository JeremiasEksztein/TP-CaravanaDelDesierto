#include <stdio.h>
#include "terminal.h"

int main(void) {
    tTerminal term;
    unsigned x, y;
    if (terminalCrear(&term)) return 1;
    if (terminalEntrarModoCrudo(&term)) return 2;
    terminalObtenerTam(&term, &x, &y);
    printf("rows=%u cols=%u\n", x, y);
    terminalSalirModoCrudo(&term);
    terminalDestruir(&term);
    return 0;
}
