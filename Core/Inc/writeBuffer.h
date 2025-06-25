#ifndef WRITEBUFFER_H
#define WRITEBUFFER_H

// Returns the length of the given null-terminated string.
int strLen(char* str);

// Concatenates the string 'tail' to the string 'head' and 
// returns a new buffer.
// The caller is responsible for freeing the returned pointer.
char* append(char* head, char* tail);

#endif // WRITEBUFFER_H
