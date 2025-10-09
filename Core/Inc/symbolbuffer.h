/*
 * symbolbuffer.h
 *
 *  Created on: Sep 28, 2025
 *      Author: florian
 */
#include "tty_types.h"
#include <stdint.h>

#ifndef INC_SYMBOLBUFFER_H_
#define INC_SYMBOLBUFFER_H_

// --- Basic manipulation ------------------------------------------
uint32_t sbf_len(sbf_t sbf);
sbf_t sbf_createSymbolBuffer(void);

sbf_t sbf_appendSym(sbf_t head, symbol_t sym);
sbf_t sbf_concaternate(sbf_t head, sbf_t tail, uint8_t keepTail);

// --- CONVERT -----------------------------------------------------
symbol_t sbf_convertToChar(symbol_t symbol, char* target, char* _newLine, uint8_t* current_mode, uint32_t* carriage_pos, uint32_t* last_lf);
sbf_t sbf_charToSymbolBuffer(sbf_t _out, char _c, tty_mode_t* _currentMode);
char* sbf_convertToString(sbf_t _inSbf,
		char* _newLine, uint8_t keepBuffer);
sbf_t sbf_convertToSymbolBuffer(char* _inStr);

#endif /* INC_SYMBOLBUFFER_H_ */
