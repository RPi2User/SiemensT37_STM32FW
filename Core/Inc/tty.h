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
    const int exclamation;
    const int lparen;
    const int rparen;
    const int slash;
    const int colon;
    const int quote;
    
    // Control
    const int ltrs;
    const int figs;
    const int null_char;
} tty_symbols_t;

// Global symbol instance for easy access
extern const tty_symbols_t symbol;

// TTY Mode enumeration
typedef enum {
    TTY_MODE_LETTERS = 0,
    TTY_MODE_FIGURES = 1
} tty_mode_t;



// Buffer manipulation
int* appendSymbol(int* head, int sym);
int getBufferLength(int* head);
int toSymbol(char c);
int* toSymbols(char* ascii_chars);
// TTY output

int* TTY_WRITEBUFFER(int* buffer);

void setTTY(int state);
void SEND_TTYC(char c);
void TTY_WRITE(int symbol);

#endif // TTY_H
