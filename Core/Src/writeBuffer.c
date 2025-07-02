#include <stdlib.h>

int strLen(char* str) {
    int n = 0;
    while (*str != '\0') {
        n++;
        str++;
    }
    return n;
}

char* toLower(char* in){
	if (in == NULL) return NULL;
	// for each char in 'in'
	for (int i = 0; in[i] != '\0'; i++){
		// is current char a Uppercase?
		if (in[i] >= 'A' && in[i] <= 'Z') 
			// offset char by distance between upper- and lowercase
			in[i] = in[i] + ('a' - 'A');
		}
	return in;
}


char* append(char* head, char* tail) {
    int len1 = strLen(head);
    int len2 = strLen(tail);
    char* newBuffer = malloc(len1 + len2 + 1);
    if (!newBuffer) return NULL;

    for (int i = 0; i < len1; i++) newBuffer[i] = head[i];
    for (int i = 0; i <= len2; i++) newBuffer[len1 + i] = tail[i];  // incl. '\0'
    free(head);
    free(tail);
    return newBuffer;
}

char* appendC(char* head, char c) {
    int len = strLen(head);
    char* newBuffer = malloc(len + 1);
    if (!newBuffer) return NULL;

    for (int i = 0; i < len; i++) newBuffer[i] = head[i];
    newBuffer[len+1] = c;
    free(head);
    return newBuffer;
}
