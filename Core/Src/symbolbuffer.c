/*
 * symbolbuffer.c
 *
 * Everything regarding teletype symbols
 *
 *  Created on: Sep 27, 2025
 *      Author: florian
 */

#include <stdlib.h>
#include <stdint.h>
#include "tty.h"
#include "symbolbuffer.h"

const int8_t SBF_TERMINATOR = -1;

// --- Basic manipulation ------------------------------------------
uint32_t sbf_len(int8_t* sbf){
	uint32_t out = 0;
	while(sbf[out] != SBF_TERMINATOR) out++;
	return out;
}

int8_t* sbf_createSymbolBuffer(void){
	// This returns a valid and correct terminated Symbol Buffer
	int8_t* _out = malloc((sizeof(int8_t)) * 1);
	if (!_out) TTY_raiseMemoryError();
	_out[0] = SBF_TERMINATOR;
	return _out;
}

int8_t* sbf_appendSym(int8_t* head, uint8_t sym){
	uint32_t head_len = 0;

	// When head NULL, create a single symbolbuffer
	if (head == NULL) {
		int8_t* tail = (int8_t*) malloc(2 * sizeof(int8_t));
		if (tail == NULL) TTY_raiseMemoryError();
		tail[0] = sym;
		tail[1] = SBF_TERMINATOR;
		return tail;
	}

	// Create a tail with correct length
	head_len = sbf_len(head);
	int8_t* tail = (int8_t*) malloc((head_len + 2) * sizeof(int8_t));
	if (tail == NULL) TTY_raiseMemoryError();

	// Populate tail
	uint32_t i = 0;
	while (head[i] != SBF_TERMINATOR) {
		tail[i] = head[i];
		i++;
	}
	// Append Symbol
	tail[i] = sym;
	tail[i+1] = SBF_TERMINATOR;
	free(head);
	return tail;
}

int8_t* sbf_concaternate(int8_t* head, int8_t* tail, uint8_t keepTail){
	uint32_t head_len = sbf_len(head);
	uint32_t tail_len = sbf_len(tail);

	int8_t* _out = malloc(sizeof(int8_t*) * (head_len + tail_len + 1));
	if (_out == NULL) TTY_raiseMemoryError();

	uint32_t len = 0;

	while(head[len] != SBF_TERMINATOR){
		_out[len] = head[len];
		len++;
	}

	while(tail[len - head_len] != SBF_TERMINATOR){
		_out[len] = tail[len - head_len];
		len++;
	}

	_out[len] = SBF_TERMINATOR;

	free(head);
	if (keepTail != 0) free(tail);
	return _out;
}

// -----------------------------------------------------------------


// --- CONVERT -----------------------------------------------------
char* sbf_convertToString(int8_t* _inSbf){
	free(_inSbf);
	return NULL;
}

int8_t* sbf_convertToSymbolBuffer(char* _inStr){
	free(_inStr);
	return NULL;
}
