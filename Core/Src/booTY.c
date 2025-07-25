#include <stdio.h> 
#include <stdlib.h>
#include "tty.h"
#include "booTY.h"
#include "main.h"


// ----------------------------------------------------------------
// SETTINGS
int terminal_width = 72;
int baudrate = 50;
int* linebreak; 
char* network_stats; 
// smth like: "+++10.10.0.222/24+fe80:aab1:22/64+++"
int* readBuffer;
// -----------------------------------------------------------------

const int VALID_MODES[] = {
	25, 19, 12, 28, 27
};

const int VALID_VALS[] = {
	22, 23, 19, 1, 10, 16, 21, 7, 6, 24
};

const int VALID_LINEBREAKS[] = {
	14, 10, 18, 13, 12
}; // `NL` for EBCDIC compatibilty

// -----------------------------------------------------------------

/* Writes Preamble to TTY
 *    - 5x cr (delay for TTY Hardware to settle)
 *    - 1x figs (for first "real" char)
 *    - 1x `?` to encourage user input
 *    - 1x ltrs to let the user in ltrs-mode
 *    - 1x space for visual appeal
 */
int* booTYinit(int* currentBuffer){
	readBuffer = (int*)malloc(1);
	HAL_Delay(50);
	TTY_WRITE(symbol.null);		// some ttys need a bit more time
	HAL_Delay(50);				// to settle
	for (int i = 0; i <= 5; i++){
		currentBuffer = appendSymbol(currentBuffer, symbol.cr);
	}
	currentBuffer = appendSymbol(currentBuffer, symbol.lf);
	currentBuffer = appendSymbol(currentBuffer, symbol.figs);
	currentBuffer = appendSymbol(currentBuffer, symbol.question);
	currentBuffer = appendSymbol(currentBuffer, symbol.ltrs);
	currentBuffer = appendSymbol(currentBuffer, symbol.space);

	return currentBuffer;
}

/* Echoes User Input
 * This function waits for a valid user input terminated by a '.'
 * and removes garbage.
 *    - when in ltrs all additional ltrs symbols will be ignored
 *    - when in figs all additional figs symbols will be ignored
 *    - all illegal chars won't be echoed
 *    - duplicate ltrs (a-z) will be ignored
 */
int* booTYshell(int* currentBuffer){
	// read user-string
	currentBuffer = readCommand(symbol.question);
	return currentBuffer;	// Returns additional stuff
}

/* This function is part of booTY it helps the user to make a correct
 * user input, by ignoring non-useable user input
 *    - reads baudot "string" terminated by a terminator of choice
 *    - helps to only allow valid mode ltrs
 *    - automagically switches between ltrs and figs for user-comfort
 */  
int* readCommand(int cmd_terminator){
	int _sym = -1;
	int _term = -1;
	do {
		_sym = TTY_READ();
		_term = TTY_READ();
		if (_term != cmd_terminator) _sym = _term;
	} while (_term != cmd_terminator);
	TTY_WRITE(cmd_terminator);	// Gives user correct symbol as response
	readBuffer = appendSymbol(readBuffer, _sym);
	return readBuffer;
}
