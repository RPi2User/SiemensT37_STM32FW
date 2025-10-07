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
#include "string.h"
#include "symbolbuffer.h"

const int8_t SBF_TERMINATOR = -1;

const int8_t SBF_MEM_ERROR[] = {
		cr, lf, ltrs, m, e, m, o, r, y, space, e, r, r, o, r, space,
		r, e, s, e, t, t, i, n, g, space, c, p, u, figs, bell, bell,
		SBF_TERMINATOR
};


// --- Private functions -------------------------------------------
tty_mode_t _findInitialMode(int8_t* _inSbf);

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

/*	Convert: symbol to char
 * 	This conversion needs a lot of contextual data. Hence why the
 * 	function head is that cluttered.
 *
 * 	This function returns
 * 	   - 2 when line-terminator should be appended
 * 	   - 1 when the char is successfully converted
 * 	   - 0 when it sym is a "dead key" like a ltrs or figs
 * 	   - -1	when a conversion error occurred
 *
 *
 */
int8_t sbf_convertToChar(int8_t symbol, char* target, char* _newLine,
   tty_mode_t* current_mode, uint32_t* carriage_pos, uint32_t* last_lf){

	if (symbol == lf) last_lf = 0;
	else last_lf++;

	// Handle common symbols
	switch(symbol){
		case cr: *carriage_pos = 0; *target = 0x0D; return 1;
		case space: (*carriage_pos)++; *target = ' '; return 1;
		case ltrs: *current_mode = TTY_LETTERS; return 0;
		case figs: *current_mode = TTY_FIGURES; return 0;
	}

	/*
	 * There are three valid cases for a new-line Insertion.
	 *  1. "CR LF"
	 *  2. "LF CR"
	 *  3. multiple "LF" (only works on carriage == 0)
	 */

	// case 1 "lfcr"
	if (*last_lf == 0 && symbol == cr){
		*carriage_pos = 0;
		*last_lf = 0;
		target = _newLine;
		return 2;
	}

	// case 2 "crlf" AND case 3 "crlflflflf..."
	if (*carriage_pos == 0 && symbol == lf){
		*carriage_pos = 0;
		*last_lf = 0;
		target = _newLine;
		return 2;
	}

	return -1;
	/*

	// if crlf OR lfcr OR lflflfl…
	if (symbol == lf && (target[i+1] == cr || carriage == 0)){
		_out = str_add(_out, _newLine, 1);
		continue;
	}

	if (_mode == TTY_LETTERS)
		_out = str_appendChar(_out, ltrs_to_char[_inSbf[i]]);
	else
		_out = str_appendChar(_out, figs_to_char[_inSbf[i]]);
	carriage++;
	*/
}

char* sbf_convertToString(int8_t* _inSbf, char* _newLine){
	char* _out = str_empty();

	if (_inSbf[0] == SBF_TERMINATOR) return _out;

	tty_mode_t _mode = TTY_LETTERS;
	// Its good practice to begin with a "ltrs" or "figs" symbol.
	// But if a leading symbol is missing, we have to assume :c
	if (_inSbf[0] != ltrs || _inSbf[0] != figs)
		_mode = _findInitialMode(_inSbf);
	else {
		if(_inSbf[0] == ltrs) _mode = TTY_LETTERS;
		else _mode = TTY_FIGURES;
	}

	uint32_t carriage = 0;	// when 0 every lf will be a newline
	for (int i = 0; _inSbf[i] != SBF_TERMINATOR; i++){

		// Handle common symbols
		switch(_inSbf[i]){
			case cr: carriage = 0; continue;
			case space: str_appendChar(_out, ' '); carriage++; continue;
			case ltrs: _mode = TTY_LETTERS; carriage++; continue;
			case figs: _mode = TTY_FIGURES; carriage++; continue;
		}

		// if crlf OR lfcr OR lflflfl…
		if (_inSbf[i] == lf && (_inSbf[i+1] == cr || carriage == 0)){
			_out = str_add(_out, _newLine, 1);
			continue;
		}

		if (_mode == TTY_LETTERS)
			_out = str_appendChar(_out, ltrs_to_char[_inSbf[i]]);
		else
			_out = str_appendChar(_out, figs_to_char[_inSbf[i]]);
		carriage++;
	}

	free(_inSbf);
	return _out;
}

int8_t* sbf_convertToSymbolBuffer(char* _inStr){
	//str_toUpper(_inStr);	// we just have a uppercase LUT
	int8_t* _out = sbf_createSymbolBuffer();
	_out = sbf_appendSym(_out, ltrs);
	tty_mode_t _currentMode = TTY_LETTERS;
	char _c = -1;
	for (uint32_t i = 0; _inStr[i] != '\0'; i++){
		_c = _inStr[i];
		if (_c <= 0x20){
			if (_c == 0x09){
				for (uint8_t t = 0; t < 4; t++)
					_out = sbf_appendSym(_out, space);
			}
			if(_c == 0x0A) _out = sbf_appendSym(_out, lf);
			if(_c == 0x0C){
				_out = sbf_appendSym(_out, cr);
				for (uint8_t f = 0; f < 10; f++)
					_out = sbf_appendSym(_out, lf);
			}
			if(_c == 0x0D) _out = sbf_appendSym(_out, cr);
			if(_c == 0x20) _out = sbf_appendSym(_out, space);
		}
		if ((_c > 0x20 && _c < 'A') || _c == 0x07 || _c > 'z'){
			if (_currentMode != TTY_FIGURES){
				_out = sbf_appendSym(_out, figs);
				_currentMode = TTY_FIGURES;
			}
			_out = sbf_appendSym(_out, char_to_symFIGS[(uint8_t)_c]);
		}
		if (_c >= 'A' && _c <= 'z'){
			if (_currentMode != TTY_LETTERS){
				_out = sbf_appendSym(_out, ltrs);
				_currentMode = TTY_LETTERS;
			}
			_out = sbf_appendSym(_out, char_to_symLTRS[(uint8_t)_c]);
		}
	}
	//free(_inStr);
	return _out;
}

// -----------------------------------------------------------------
// PRIVATE FUNCTIONS
void ___();
tty_mode_t _findInitialMode(int8_t* _inSbf){
	for (uint32_t i = 0; _inSbf[i] != SBF_TERMINATOR; i++){
		if (_inSbf[i] == ltrs) return TTY_FIGURES;
		if (_inSbf[i] == figs) return TTY_LETTERS;
	}
	return TTY_LETTERS;	// assume default value
}
