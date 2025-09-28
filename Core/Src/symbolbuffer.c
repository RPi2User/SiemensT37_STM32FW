/*
 * symbolbuffer.c
 *
 * This interface handles any
 *
 *  Created on: Sep 27, 2025
 *      Author: florian
 */

#include <stdlib.h>
#include <stdint.h>
#include "tty.h"

const int8_t SBF_TERMINATOR = -1;


// -----------------------------------------------------------------
int8_t* sbf_createSymbolBuffer(void){
	// This returns a valid and correct terminated Symbol Buffer
	int8_t* _out = malloc((sizeof(int8_t)) * 1);
	if (!_out) TTY_raiseMemoryError();
	_out[0] = SBF_TERMINATOR;
	return _out;
}

// This returns the length of the sbf INCLUDING Terminator
uint32_t sbf_getLength(const int8_t* _inSbf) {
    uint32_t _out = 0;
    while (_inSbf[_out] != SBF_TERMINATOR) _out++;
    return _out;
}

// --- CONVERT -----------------------------------------------------
char* sbf_convertToString(int8_t* _inSbf){
	free(_inSbf);
	return NULL;
}

int8_t* sbf_convertToSymbolBuffer(char* _inStr){
	free(_inStr);
	return NULL;
}
