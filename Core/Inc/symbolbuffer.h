/*
 * symbolbuffer.h
 *
 *  Created on: Sep 28, 2025
 *      Author: florian
 */

#ifndef INC_SYMBOLBUFFER_H_
#define INC_SYMBOLBUFFER_H_

int8_t* sbf_createSymbolBuffer(void);
uint32_t sbf_getLength(const int8_t* _inSbf);

char* sbf_convertToString(int8_t* _inSbf);
int8_t* sbf_convertToSymbolBuffer(char* _inStr);

#endif /* INC_SYMBOLBUFFER_H_ */
