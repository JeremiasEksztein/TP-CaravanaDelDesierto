#include <stdio.h>

#include "terminal.h"
#include "canvas.h"

int main(void)
{
	tTerminal term;
	unsigned x, y;
	tCanvas canvas;

	if(terminalCrear(&term)) {
		return 1;
	}

	terminalEntrarModoCrudo(&term);

	terminalObtenerTam(&term, &x, &y);

	canvasCrear(&canvas, x, y);


	canvasDestruir(&canvas);
	terminalSalirModoCrudo(&term);
	terminalDestruir(&term);

	return 0;
}
