#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "tty.h"

char FOX[] = {'\r', '\n', 't', 'h', 'e', ' ', 'q', 'u', 'i', 'c', 'k',
			' ', 'b', 'r', 'o', 'w', 'n', ' ', 'f', 'o', 'x', ' ',
			'j', 'u', 'm', 'p', 's', ' ', 'o', 'v', 'e', 'r', ' ',
			't', 'h', 'e', ' ', 'l', 'a', 'z', 'y', ' ', 'd', 'o',
			'g', '\0'};

// Teletype Variables
int tty_mode = TTY_MODE_LETTERS;    // Init to TTY-Mode LTRS
int send_mode = 0;		// flag to send current mode again
int loopback = 0;		// This sends bit right back to TTY

int baud = 50;			// default Baudrate for TTYs
int width = 72;			// terminal width

int READ_TIMEOUT = 1000;// Timeout of 1000ms
float stopbit_cnt = 1.5;// booTY will be setting this correctly


// TTY Symbol definitions with decimal values
const tty_symbols_t symbol = {
    // Letters (a-z) - decimal values
    .a = 3,     // 0b00011
    .b = 25,    // 0b11001
    .c = 14,    // 0b01110
    .d = 9,     // 0b01001
    .e = 1,     // 0b00001
    .f = 13,    // 0b01101
    .g = 26,    // 0b11010
    .h = 20,    // 0b10100
    .i = 6,     // 0b00110
    .j = 11,    // 0b01011
    .k = 15,    // 0b01111
    .l = 18,    // 0b10010
    .m = 28,    // 0b11100
    .n = 12,    // 0b01100
    .o = 24,    // 0b11000
    .p = 22,    // 0b10110
    .q = 23,    // 0b10111
    .r = 10,    // 0b01010
    .s = 5,     // 0b00101
    .t = 16,    // 0b10000
    .u = 7,     // 0b00111
    .v = 30,    // 0b11110
    .w = 19,    // 0b10011
    .x = 29,    // 0b11101
    .y = 21,    // 0b10101
    .z = 17,    // 0b10001
    
    // Numbers (0-9) 
    .n0 = 22,   // 0b10110 (P in LTRS)
    .n1 = 23,   // 0b10111 (Q in LTRS)
    .n2 = 19,   // 0b10011 (W in LTRS)
    .n3 = 1,    // 0b00001 (E in LTRS)
    .n4 = 10,   // 0b01010 (R in LTRS)
    .n5 = 16,   // 0b10000 (T in LTRS)
    .n6 = 21,   // 0b10101 (Y in LTRS)
    .n7 = 7,    // 0b00111 (U in LTRS)
    .n8 = 6,    // 0b00110 (I in LTRS)
    .n9 = 24,   // 0b11000 (O in LTRS)
    
    // Special characters
    .space = 4,        // 0b00100
    .cr = 8,           // 0b01000
    .lf = 2,           // 0b00010
    .period = 28,      // 0b11100 (M in LTRS)
    .comma = 12,       // 0b01100 (N in LTRS)
    .minus = 3,        // 0b00011 (A in LTRS)
    .plus = 17,        // 0b10001 (Z in LTRS)
    .question = 25,    // 0b11001 (B in LTRS)
    .lparen = 15,      // 0b01111 (K in LTRS)
    .rparen = 18,      // 0b10010 (L in LTRS)
    .slash = 29,       // 0b11101 (V in LTRS)
    .colon = 14,       // 0b01110 (C in LTRS)
    .quote = 5,        // 0b00101 (S in LTRS)
    
    // Control
    .ltrs = 31,   // 0b11111
    .figs = 27,   // 0b11011
    .null = 0     // 0b00000
};

// ---BUFFER MANIPULATION-------------------------------------------
int* appendSymbol(int* head, int sym){
	// Handle NULL head pointer
	if (head == NULL) {
		int* out = (int*)malloc(2);
		if (out == NULL) return NULL;  // malloc failed, 2nd Try
		out[0] = sym;
		out[1] = -1;
		return out;
	}
	
	int length = getBufferLength(head);
	// length + Symbol + Terminator
	int* out = (int*)malloc((length + 2) * sizeof(int));
	if (out == NULL) {
		// malloc failed - don't free head, return NULL
		return NULL;
	}
	
	// Copy existing elements...
	for (int i = 0; i < length; i++) {
		out[i] = head[i];
	}
	out[length] = sym;        // Add symbol at correct position
	out[length + 1] = -1;     // Add terminator

	free(head);	// the old head can be removed
	return out;
}
int* appendChar(int* head, char c){
    int sym = toSymbol(c);

    // if line width is reached:
    if (getBufferLength(head) < width){
    	head = appendSymbol(head, symbol.cr);
    	head = appendSymbol(head, symbol.lf);
    }

    // need to check for line termination and term-width
    // when "crlf" or "cr" or "lf" or "nl" always -> "cr" + "lf""
    return appendSymbol(head, sym);
}

int getBufferLength(int* head){	// returns without Terminator!
	if (head == NULL) return 0;  // Safety check
	
	int i = 0;
	while (head[i] != -1){
		i++;
	}
	return i;
}

int toSymbol(char c) {
    static const unsigned char lut[128] = {
        [0x07] = 11,    // BELL
        [0x0A] = 2,     // \n
        [0x0D] = 8,     // \r
        [0x11] = 9,     // DC1 (WhoThere?)
        [' ']  = symbol.space,
        ['\''] = 5,
        ['(']  = 15,
        [')']  = 18,
        [',']  = 12,
        ['-']  = 3,
        ['.']  = 28,
        ['/']  = 29,
        ['0']  = 22,
        ['1']  = 23,
        ['2']  = 19,
        ['3']  = 1,
        ['4']  = 10,
        ['5']  = 16,
        ['6']  = 21,
        ['7']  = 7,
        ['8']  = 6,
        ['9']  = 24,
        [':']  = 14,
        ['=']  = 30,
        ['?']  = 25,
        ['A']  = 3,  ['a'] = 3,
        ['B']  = 25, ['b'] = 25,
        ['C']  = 14, ['c'] = 14,
        ['D']  = 9,  ['d'] = 9,
        ['E']  = 1,  ['e'] = 1,
        ['F']  = 13, ['f'] = 13,
        ['G']  = 26, ['g'] = 26,
        ['H']  = 20, ['h'] = 20,
        ['I']  = 6,  ['i'] = 6,
        ['J']  = 11, ['j'] = 11,
        ['K']  = 15, ['k'] = 15,
        ['L']  = 18, ['l'] = 18,
        ['M']  = 28, ['m'] = 28,
        ['N']  = 12, ['n'] = 12,
        ['O']  = 24, ['o'] = 24,
        ['P']  = 22, ['p'] = 22,
        ['Q']  = 23, ['q'] = 23,
        ['R']  = 10, ['r'] = 10,
        ['S']  = 5,  ['s'] = 5,
        ['T']  = 16, ['t'] = 16,
        ['U']  = 7,  ['u'] = 7,
        ['V']  = 30, ['v'] = 30,
        ['W']  = 19, ['w'] = 19,
        ['X']  = 29, ['x'] = 29,
        ['Y']  = 21, ['y'] = 21,
        ['Z']  = 17, ['z'] = 17,
        ['+']  = 17,
    };
    if ((unsigned char)c < 128)
        return lut[(unsigned char)c];
    return 0;
}


// ---CHARACTER CONVERSION-------------------
char toChar(int symbol){

	if (tty_mode == TTY_MODE_LETTERS){
		return toCharLTRS(symbol);
	}
	else {
		return toCharFIGS(symbol);
	}
}

char toCharLTRS(int sym){
    // Lookup table for LETTERS mode (LTRS)
    static const char ltrs_to_char[32] = {
        [1]  = 'e',   // E
        [3]  = 'a',   // A
        [5]  = 's',   // S
        [6]  = 'i',   // I
        [7]  = 'u',   // U
        [9]  = 'd',   // D
        [10] = 'r',   // R
        [11] = 'j',   // J
        [12] = 'n',   // N
        [13] = 'f',   // F
        [14] = 'c',   // C
        [15] = 'k',   // K
        [16] = 't',   // T
        [17] = 'z',   // Z
        [18] = 'l',   // L
        [19] = 'w',   // W
        [20] = 'h',   // H
        [21] = 'y',   // Y
        [22] = 'p',   // P
        [23] = 'q',   // Q
        [24] = 'o',   // O
        [25] = 'b',   // B
        [26] = 'g',   // G
        [28] = 'm',   // M
        [29] = 'x',   // X
        [30] = 'v',   // V
        // |ltrs_to_char| = 15
    };
    
    if (sym < 0 || sym > 31) return '\0';
    return ltrs_to_char[sym];
}

char toCharFIGS(int sym){
    // Lookup table for FIGURES mode (FIGS)
    static const char figs_to_char[32] = {
        [1]  = '3',   // 3 (E in LTRS)
        [3]  = '-',   // - (A in LTRS)
        [5]  = '\'',  // ' (S in LTRS)
        [6]  = '8',   // 8 (I in LTRS)
        [7]  = '7',   // 7 (U in LTRS)
        [9]  = '\0',  // WRU/ENQ (D in LTRS)
        [10] = '4',   // 4 (R in LTRS)
        [11] = '\a',  // BELL (J in LTRS)
        [12] = ',',   // , (N in LTRS)
        [14] = ':',   // : (C in LTRS)
        [15] = '(',   // ( (K in LTRS)
        [16] = '5',   // 5 (T in LTRS)
        [17] = '+',   // + (Z in LTRS)
        [18] = ')',   // ) (L in LTRS)
        [19] = '2',   // 2 (W in LTRS)
        [21] = '6',   // 6 (Y in LTRS)
        [22] = '0',   // 0 (P in LTRS)
        [23] = '1',   // 1 (Q in LTRS)
        [24] = '9',   // 9 (O in LTRS)
        [25] = '?',   // ? (B in LTRS)
        [28] = '.',   // . (M in LTRS)
        [29] = '/',   // / (X in LTRS)
        [30] = '=',   // = (V in LTRS)
    }; // |figs_to_char| = 22
    
    if (sym < 0 || sym > 31) return '\0';
    return figs_to_char[sym];
}

// -----------------------------------------------------------------

// Debug function prints a brown fox
int* TTY_FOX(int* buffer){
	for (int i = 0; i < 45; i++){
		buffer = appendChar(buffer, FOX[i]);
	}
	return buffer;
}


// ---TTY-FUNCTIONS-------------------------------------------------
void TTY_DELAY(float cycles){
	HAL_Delay((int)(cycles * ( 1000 / baud)));
}

int* TTY_WRITEBUFFER(int* buffer){
    
    // Write all symbols in buffer
    for (int i = 0; buffer[i] != -1; i++) {
        TTY_WRITE(buffer[i]);
    }
    
    // Free the input buffer
    free(buffer);
    
    // Create new empty buffer
    int* out = (int*)malloc(sizeof(int));
    if (out == NULL) {
        return NULL;  // malloc failed - caller must handle this!
    }
    out[0] = -1;  // Null-terminate
    return out;
}


void TTY_Startbit(){
	setTTY(1);
	TTY_DELAY(1.0);
}

void TTY_Stopbit(){
	setTTY(0);
	TTY_DELAY(stopbit_cnt);
}
void TTY_WRITE(int _sym){

	if (_sym == -1) return;

	// Skip redundant ltrs/figs commands
	if (_sym == TTY_MODE_FIGURES || _sym == TTY_MODE_LETTERS)
		tty_mode = TTY_MODE_FIGURES ?
				TTY_MODE_FIGURES : TTY_MODE_LETTERS;

	// if we want to use a "ltrs"|"figs" as audio-visual thingie
	// then we set this self-resetting flag
	if (send_mode != 0){
		if (tty_mode == TTY_MODE_FIGURES) _sym = symbol.figs;
		else _sym = symbol.ltrs;
		send_mode = 0;			// Remove Flag
	}
    // ---TRANSMIT--------------------------------------------------
	TTY_Startbit();

	// LSB FIRST!
    for (int i = 0; i < 5; i++) {
        int bit = ((_sym >> i) & 0x01) ^ 1;
        setTTY(bit);
        TTY_DELAY(1);
    }
	// send those 5 bits
	TTY_Stopbit();
}

/*	==== READ OPERATIONS ===========================================
 *
 * This section contains all functions regarding reading Data from
 * the Teletype. I tried to keep this section clean but some stuff
 * is quite necessary for a stable "API" like infrastructure.
 *
 *	- TTY_READ() is the main function, this reads a single Symbol
 *				 sensitive to current LTRS | FIGS mode from the TTY
 */

char TTY_READ(){
	int sym0 = readSymbol();

    // Common Symbols
    switch(sym0){
	    case -1: return '\0';break; // NULL if ERR
        case 0: return '\0'; break; // NULL is ignored
        case 2: return '\n'; break; // NewLine if LineFeed
        case 4: return ' ';  break; // ' ' if SPACE
        case 27: {
            if (tty_mode == TTY_MODE_FIGURES)
                return '\0'; // Can be DC2-4 in future
        } break;
        case 31: {
            if (tty_mode == TTY_MODE_LETTERS)
                return '\0'; // Can be DC2-4 in future
        } break;
    }

    return toChar(sym0);

}


int readSymbol() {
	// wait for Symbol-Trigger
	for (int i = 0; i <= READ_TIMEOUT; i+=2){
		if (readTTY() == 0) HAL_Delay(2);
		else break;
	}
	// read start-bit
	// pattern: 20ms startbit, 5x20ms Databit, 1.n stopbits
	// STARTBIT is 20ms HIGH
	HAL_Delay(9);	// Wait 10ms
	int beg = readTTY();
	HAL_Delay(14);	// Wait 5 + 10ms
	// NOTE: the additional delay (above) is the SAMPLING OFFSET!

	Databit databit[5];
	for (int i = 0; i < 5; i++){
		databit[i].s1 = readTTY();
		HAL_Delay(4); // 5ms Delay
		databit[i].s2 = readTTY();
		HAL_Delay(4); // 5ms Delay
		databit[i].s3 = readTTY();
		HAL_Delay(9); // 10ms delay
	}

	// END Bit low for 1.5|1 sub-symbols
	// Should be 20-30ms
	// we use 5ms to determine whether or not a stopbit exists
	HAL_Delay(5);	// get some air for calculations

	// we can skip entire stopbit business, we are slow af
	// --- BUG? ---
	// well that might not aged that well…
	// Loopback: after an ltrs, an additional ltrs got detected/send.
	// Assumption: all calculations should be done after 2ms.
	// Solution: just before we return the value we wait the entire
	//           Stopbit / "Pausenschritt" as Siemens calls it :3

	int end = readTTY();

	// Eval Bits
	if (beg == 0 || end != 0){
		setReadError();
		return -1;
	}

	// BUG: Dauerfeuer von y funktioniert, r's werden immernoch falsch erkannt!

    int out = 0;

    // Weird bug with bit concatenation ig.
    if (majority(databit[0]) == 0) out += 1;
    if (majority(databit[1]) == 0) out += 2;
    if (majority(databit[2]) == 0) out += 4;
    if (majority(databit[3]) == 0) out += 8;
    if (majority(databit[4]) == 0) out += 16;

    clearReadError();

    HAL_Delay(10);	// Stobbit should be <20ms
    return out;
}

int majority(Databit d) {
    return (d.s1 + d.s2 + d.s3) >= 2 ? 1 : 0;
}

// This probes the TTY_RECV Pin
int readTTY(){
	int out = -1;
	out = HAL_GPIO_ReadPin(GPIOB, TTY_RECV_Pin);
	if (loopback != 0) setTTY(out);
	return out;
}

void setTTY(int state){			// TTY @ A3
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}

void TTY_DEBUG(){
}

void setReadError(){ HAL_GPIO_WritePin(GPIOA, TTY_READERR_Pin, 1); }
void clearReadError(){ HAL_GPIO_WritePin(GPIOA, TTY_READERR_Pin, 0); }

// Settings Interface for booTY
void setLoopback(int _loopback) {loopback = _loopback;}
void setBaudrate(int baudrate) {baud = baudrate;}
void setTermWidth(int termwidth) {width = termwidth;}
void setStopbits(float stopbit) {stopbit_cnt = stopbit;}
void setSendMode(){send_mode = 1;}
// TODO add Lower/Uppercase "switch"
