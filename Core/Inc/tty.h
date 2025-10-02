#include <stdint.h>

#ifndef TTY_H
#define TTY_H

typedef struct {
    int s1;
    int s2;
    int s3;
} Databit;

// TTY Symbols Structure for easy access
typedef struct {
    // Letters (lowercase for easy access)
    const int a;
    const int b;
    const int c;
    const int d;
    const int e;
    const int f;
    const int g;
    const int h;
    const int i;
    const int j;
    const int k;
    const int l;
    const int m;
    const int n;
    const int o;
    const int p;
    const int q;
    const int r;
    const int s;
    const int t;
    const int u;
    const int v;
    const int w;
    const int x;
    const int y;
    const int z;
    
    // Numbers
    const int n0;
    const int n1;
    const int n2;
    const int n3;
    const int n4;
    const int n5;
    const int n6;
    const int n7;
    const int n8;
    const int n9;
    
    // Special characters
    const int space;
    const int cr;
    const int lf;
    const int period;
    const int comma;
    const int minus;
    const int plus;
    const int question;
    const int lparen;
    const int rparen;
    const int slash;
    const int colon;
    const int quote;
    
    // Control
    const int ltrs;
    const int figs;
    const int null;
} tty_symbols_t;

extern const tty_symbols_t symbol;


// TTY Mode enumeration
// maybe useless
typedef enum {
    TTY_LETTERS = 0,
    TTY_FIGURES = 1
} tty_mode_t;

// Conversion -> Will be moved to sbf.h
uint8_t toSymbol(char c);
char toCharLTRS(uint8_t sym);
char toCharFIGS(uint8_t sym);
char toChar(int8_t _symbol);

// Buffer manipulation
int8_t* appendSymbol(int8_t* head, int8_t sym);
int8_t* appendChar(int8_t* head, char c);
uint8_t getBufferLength(int8_t* head);

void TTY_FOX(void);

void TTY_WRITEKEY(char key);
void TTY_WRITESTRING(char* str);
int8_t* TTY_WRITEBUFFER(int8_t* buffer);

void TTY_Write(int8_t _sym);


char TTY_READKEY();
int8_t readSymbol();

uint8_t majority(Databit d);

int8_t readTTY();
void setTTY(uint8_t state);

void TTY_DELAY(float cycles);
void TTY_Startbit();
void TTY_Stopbit();

void TTY_raiseMemoryError(void);
void setReadError();
void clearReadError();

void setLoopback(uint8_t _loopback);
void setBaudrate(uint8_t baudrate);
void setTermWidth(uint8_t termwidth);
void setStopbits(float stopbit);
void setSendMode();

#endif // TTY_H
