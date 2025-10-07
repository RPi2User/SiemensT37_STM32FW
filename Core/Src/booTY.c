#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include "tty.h"
#include "booTY.h"
#include "main.h"
#include "symbolbuffer.h"


// ----------------------------------------------------------------
// SETTINGS
uint8_t terminal_width = 72;
int8_t* linebreak;
char* network_stats; 
// smth like: "+++10.10.0.222/24+fe80:aab1:22/64+++"
int8_t* readBuffer;
// -----------------------------------------------------------------

const int8_t VALID_MODES[] = {
	25, 19, 12, 28, 27
}; // [b]aud, [w]idth, [n]et, [.]term, figs

const int8_t VALID_VALS[] = {
	22, 23, 19, 1, 10, 16, 21, 7, 6, 24
};

const int8_t VALID_LINEBREAKS[] = {
	14, 10, 18, 13, 12 //[c][r], [l][f],[n]l
}; // `NL` for EBCDIC compatibilty

// -----------------------------------------------------------------

/* Writes Preamble to TTY
 *    - 5x cr (delay for TTY Hardware to settle)
 *    - 1x figs (for first "real" char)
 *    - 1x `?` to encourage user input
 *    - 1x ltrs to let the user in ltrs-mode
 *    - 1x space for visual appeal
 */
int8_t* booTYinit(int8_t* currentBuffer){
	readBuffer = (int8_t*)malloc(1);
	HAL_Delay(50);
	TTY_Write(null);		// some ttys need a bit more time
	HAL_Delay(50);				// to settle

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
int8_t* booTYshell(int8_t* currentBuffer){
	/* 1. Time runs out or ONE key get pressed
	 * 2. Users are only allowed to press correct keys
	 * 	  all others will get rejected
	 * 3. After successful config print RDY
	 */
	return NULL;
}

/* This function is part of booTY it helps the user to make a correct
 * user input, by ignoring non-useable user input
 *    - reads baudot "string" terminated by a terminator of choice
 *    - helps to only allow valid mode ltrs
 *    - automagically switches between ltrs and figs for user-comfort
 */  
int8_t readCommand(int8_t cmd_terminator){
	int8_t _sym = -1;
	int8_t _term = -1;
	do {
		_sym = readSymbol();
		_term = readSymbol();
		if (_term != cmd_terminator) _sym = _term;
	} while (_term != cmd_terminator);
	TTY_Write(cmd_terminator);	// Gives user correct symbol as response
	return _sym;
}
