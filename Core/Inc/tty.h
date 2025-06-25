#ifndef TTY_H
#define TTY_H

void TTY_DELAY(int cycles);
void TTY_WRITE(int symbol);
void ryLoop();
int convertToTTY(char* c);
void SEND_TTYC(char c);


#endif // TTY_H
