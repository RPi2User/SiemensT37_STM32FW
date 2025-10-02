/*
 * string.h
 *
 *  Created on: Sep 29, 2025
 *      Author: florian
 */

#ifndef INC_STRING_H_
#define INC_STRING_H_

uint32_t str_len(char* str);
char* str_empty(void);

char* str_appendChar(char* head, char c);
char* str_add(char* head, char* tail, uint8_t keepTail);

void str_toUpper(char* str);
void str_toLower(char* str);

#endif /* INC_STRING_H_ */
