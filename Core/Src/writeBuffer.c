#include <stdlib.h>

int strLen(char* str) {
    int n = 0;
    while (*str != '\0') {
        n++;
        str++;
    }
    return n;
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
