#include <stdint.h>

#ifndef TTY_H
#define TTY_H

extern const int8_t SBF_MEM_ERROR[];

typedef struct {
    int s1;
    int s2;
    int s3;
} Databit;

// TTY Mode enumeration
typedef enum {
    TTY_LETTERS = 0,
    TTY_FIGURES = 1
} tty_mode_t;

void TTY_Init();

void TTY_FOX(void);

void TTY_WriteKey(char key);
void TTY_WriteString(char* str, uint8_t keepStr);
int8_t* TTY_WriteBuffer(int8_t* buffer);

void TTY_Write(int8_t _sym);


char TTY_READKEY();
int8_t readSymbol();

uint8_t majority(Databit d);

int8_t readTTY();
void setTTY(uint8_t state);
void TTY_DelayMS(uint32_t ms);
void TTY_Delay(float cycles);
void TTY_Startbit();
void TTY_Stopbit();

void TTY_raiseMemoryError(void);
void setReadError();
void clearReadError();

void setLoopback(uint8_t _loopback);
void setBaudrate(float baudrate);
void setTermWidth(uint8_t termwidth);
void setStopbits(float stopbit);

#endif // TTY_H
