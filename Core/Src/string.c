/*
 * string.c
 *
 *  Created on: Sep 29, 2025
 *      Author: florian
 */


#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <tty.h>

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

uint32_t str_len(char* str){
	uint32_t i = 0;
	while(str[i] != '\0') i++;
	return i;
}
char* str_empty(void){
	char* _out = (char*) malloc(1 * sizeof(char));
	if (_out == NULL) TTY_raiseMemoryError();
	_out[0] = '\0';
	return _out;

}

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



char* str_add(char* head, char* tail, uint8_t keepTail){
	uint32_t head_len = str_len(head);
	uint32_t tail_len = str_len(tail);
	uint32_t len = 0;

	char* _out = (char*) malloc((head_len + tail_len + 1) * sizeof(char));
	if (_out == NULL) TTY_raiseMemoryError();

	while(head[len] != '\0'){
		_out[len] = head[len];
		len++;
	}

	while(tail[len - head_len] != '\0'){
		_out[len] = tail[len - head_len];
		len++;
	}
	_out[len] = '\0';

	free(head);
	if (keepTail != 0) free(tail);

	return _out;
}

void str_toUpper(char* str){
	for (int i = 0; str[i] != '\0'; i++){
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 'a' - 'A';
	}
}

void str_toLower(char* str){
	for (int i = 0; str[i] != '\0'; i++){
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 'a' - 'A';
	}
}


