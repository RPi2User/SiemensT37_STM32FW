// ---STR MANIPULATION----------------------------------------------
int strLen(char* str){
	int n = 0;
	while(*str != '\0') {
		n++;
		str++;
	}
	return n;
}

// This appends Tail to writeBuffer
void append(char* tail){
    int len1 = strLen(writeBuffer);
    int len2 = strLen(tail);
    char* newBuffer = malloc(len1 + len2 + 1);
    if (!newBuffer) return;

    for (int i = 0; i < len1; i++) newBuffer[i] = writeBuffer[i];
    for (int i = 0; i <= len2; i++) newBuffer[len1 + i] = tail[i];  
	// inkl. '\0'

    free(writeBuffer);
    free(tail);
    writeBuffer = newBuffer;
}
// -----------------------------------------------------------------