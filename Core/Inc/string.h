/*
 * string.h
 *
 *  Created on: Sep 29, 2025
 *      Author: florian
 */

#ifndef INC_STRING_H_
#define INC_STRING_H_

char* str_append(char* head, char c);
char* str_add(char* head, char* tail);
void str_toUpper(char* str);
void str_toLower(char* str);
uint32_t str_len(char* str);

#endif /* INC_STRING_H_ */
