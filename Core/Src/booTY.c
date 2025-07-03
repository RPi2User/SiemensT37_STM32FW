#include <stdio.h> 
#include <stdlib.h>
#include "tty.h"


// Writes Preamble to TTY
int* init(int* currentBuffer){

	for (int i = 0; i <= 5; i++)
		currentBuffer = appendSymbol(currentBuffer, symbol.ltrs);
	currentBuffer = appendSymbol(currentBuffer, symbol.figs);
	currentBuffer = appendSymbol(currentBuffer, symbol.question);

	return currentBuffer;
}

void 

