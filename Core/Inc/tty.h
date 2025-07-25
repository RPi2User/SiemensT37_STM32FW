#ifndef TTY_H
#define TTY_H


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
    TTY_MODE_LETTERS = 0,
    TTY_MODE_FIGURES = 1
} tty_mode_t;



// Buffer manipulation
int* appendSymbol(int* head, int sym);
int getBufferLength(int* head);
int toSymbol(char c);
int* toSymbols(char* ascii_chars);

// Read Section
void setReadError();
void clearReadError();
int TTY_READ();
int readTTY();
int readSymbol();

// TTY Main output
int* TTY_WRITEBUFFER(int* buffer);


// Write Section
int* TTY_FOX(int* buffer);

void TTY_WRITE(int symbol);
void TTY_SEND(int bit, int cycles);

void TTY_Startbit();
void TTY_Stopbit();

void TTY_DEBUG();

// Program Vars
void setTTY(int state);
void setBaudrate(int baudrate);
void setTermWidth(int termwidth);
void setStopbits(float stopbit);
void setSendMode();
void setInterrupt();

#endif // TTY_H
