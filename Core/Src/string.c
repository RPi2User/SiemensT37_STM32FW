/*
 * string.c
 *
 *  Created on: Sep 29, 2025
 *      Author: florian
 */


#include <stdlib.h>

/* Functions
 *
 *  - char* str_append(char* head, char c)
 *  - char* str_add(char* head, char* tail)
 *  - void str_toUpper(char* str)
 *  - void str_toLower(char* str)
 *  - int str_len(char* str)
 *
 *
 */

char* str_appendChar(char* head, char c){
	uint32_t head_len = 0;

	// When head NULL, create a single string
	if (head == NULL) {
		char* tail = (char*) malloc(2 * sizeof(char));
		if (tail == NULL) TTY_raiseMemoryError();
		tail[0] = c;
		tail[1] = '\0';
		return tail;
	}

	head_len = str_len(head);
	char* tail = (char*) malloc((head_len + 2) * sizeof(char));
	if (tail == NULL) TTY_raiseMemoryError();

	uint32_t i = 0;
	while (head[i] != '\0') {
		tail[i] = head[i];
		i++;
	}
	tail[i] = c;
	tail[i+1] = '\0';

	return tail;
}

uint32_t str_len(char* str){
	uint32_t i = 0;
	while(str[i] != '\0') i++;
	return i;
}

char* str_add(char* str1, char* str2){
	uint32_t str1_len = str_len(str1);
	uint32_t str2_len = str_len(str2);
	uint32_t len = 0;

	char* _out = (char*) malloc((str1_len + str2_len + 1) * sizeof(char));
	if (_out == NULL) TTY_raiseMemoryError();

	while(str1[len] != '\0'){
		_out[len] = str1[len];
		len++;
	}

	while(str2[len - str1_len] != '\0'){
		_out[len] = str2[len - str1_len];
		len++;
	}
	_out[len] = '\0';
	return _out;
}

void str_toUpper(char* str){

}

void str_toLower(char* str){

}


