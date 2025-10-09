/*
 * symbolbuffer.h
 *
 *  Created on: Sep 28, 2025
 *      Author: florian
 */
#include "tty_types.h"

#ifndef INC_SYMBOLBUFFER_H_
#define INC_SYMBOLBUFFER_H_

// --- Basic manipulation ------------------------------------------
uint32_t sbf_len(sbf sbf);
int8_t* sbf_createSymbolBuffer(void);

int8_t* sbf_appendSym(sbf head, symbol sym);
int8_t* sbf_concaternate(sbf head, sbf tail, uint8_t keepTail);

// --- CONVERT -----------------------------------------------------
int8_t sbf_convertToChar(int8_t symbol, char* target, char* _newLine, uint8_t* current_mode, uint32_t* carriage_pos, uint32_t* last_lf);
int8_t* sbf_charToSymbolBuffer(int8_t* _out, char _c, tty_mode_t* _currentMode);
char* sbf_convertToString(int8_t* _inSbf,
		char* _newLine, uint8_t keepBuffer);
int8_t* sbf_convertToSymbolBuffer(char* _inStr);

#endif /* INC_SYMBOLBUFFER_H_ */
