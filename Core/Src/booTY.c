#include <stdio.h> 
#include <stdlib.h>
#include "tty.h"


// ----------------------------------------------------------------
// SETTINGS
int terminal_width = 72;
int baudrate = 50;
int* linebreak; 
char* network_stats; 
// smth like: "+++10.10.0.222/24+fe80:aab1:22/64+++"

// -----------------------------------------------------------------

const int VALID_MODES[] = {
	symbol.b, symbol.w, symbol.n, symbol.period, symbol.figs
}:

const int VALID_VALS[] = {
	symbol.n0, symbol.n1, symbol.n2, symbol.n3, symbol.n4, 
	symbol.n5, symbol.n6, symbol.n7, symbol.n8, symbol.n9
};

const int VALID_LINEBREAKS[] = {
	symbol.c, symbol.r, symbol.l, symbol.f, symbol.n
}; // `NL` for EBCDIC compatibilty

// -----------------------------------------------------------------

/* Writes Preamble to TTY
 *    - 5x ltrs (delay for TTY Hardware to settle)
 *    - 1x figs (for first "real" char)
 *    - 1x `?` to encourage user input
 *    - 1x ltrs to let the user in ltrs-mode
 *    - 1x space for visual appeal
 */
int* booTYinit(int* currentBuffer){
	for (int i = 0; i <= 5; i++)
		currentBuffer = appendSymbol(currentBuffer, symbol.ltrs);
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
	return currentBuffer;
}

/* This function is part of booTY it helps the user to make a correct
 * user input, by ignoring non-useable user input
 *    - reads baudot "string" terminated by a terminator of choice
 *    - helps to only allow valid mode ltrs
 *    - automagically switches between ltrs and figs for user-comfort
 */  
int* readCommand(int cmd_terminator){

}

int readKey(){
}


// prints stuff to TTY
void fart(int symbol){
	TTY_WRITE(symbol);
}

