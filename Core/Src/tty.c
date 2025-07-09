#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "tty.h"

// Teletype Variables
int rx_figs = 0;    // whether or not currently in figs or ltrs mode
int tx_figs = 0;    	// ebd.
int baud = 20;			// default Baudrate for TTYs
int width = 72;			// terminal width
int newLineSymbols = 0; // 0 = CRLF, 1 = CR, 2 = LF, 3 = NL 
int previousBit = 0;	// used for duty-cycle control TODO DEPRECATE THIS

float stopbit_cnt = 1.5;// count of stopbits.
						// booTY need to take care of this!


// TTY Symbol definitions with decimal values
const tty_symbols_t symbol = {
    // Letters (a-z) - decimal values
    .a = 3,    // 0b00011
    .b = 25,    // 0b11001  
    .c = 14,    // 0b01110
    .d = 9,    // 0b01001
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
    .n1 = 29,   // 0b11101 (Q in LTRS)
    .n2 = 19,   // 0b10011 (W in LTRS)
    .n3 = 1,    // 0b00001 (E in LTRS)
    .n4 = 10,   // 0b01010 (R in LTRS)
    .n5 = 16,   // 0b10000 (T in LTRS)
    .n6 = 21,   // 0b10101 (Y in LTRS)
    .n7 = 28,   // 0b11100 (U in LTRS)
    .n8 = 6,    // 0b00110 (I in LTRS)
    .n9 = 3,    // 0b00011 (O in LTRS)
    
    // Special characters
    .space = 4,        // 0b00100
    .cr = 8,           // 0b01000
    .lf = 2,           // 0b00010
    .period = 7,       // 0b00111 (M in LTRS)
    .comma = 12,       // 0b01100 (N in LTRS)
    .minus = 24,       // 0b11000 (A in LTRS)
    .plus = 17,        // 0b10001 (Z in LTRS)
    .question = 19,    // 0b10011 (B in LTRS)
    .exclamation = 26, // 0b11010 (G in LTRS)
    .lparen = 15,      // 0b01111 (K in LTRS)
    .rparen = 9,       // 0b01001 (L in LTRS)
    .slash = 30,       // 0b11110 (V in LTRS)
    .colon = 14,       // 0b01110 (C in LTRS)
    .quote = 13,       // 0b01101 (F in LTRS)
    
    // Control
    .ltrs = 31,        // 0b11111
    .figs = 27,        // 0b11011
    .null_char = 0     // 0b00000
};

// ---BUFFER MANIPULATION-------------------------------------------
int* appendSymbol(int* head, int sym){
	// Handle NULL head pointer
	if (head == NULL) {
		int* out = (int*)malloc(2);
		if (out == NULL) return NULL;  // malloc failed
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
        [' ']  = 4,
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
// -----------------------------------------------------------------



// ---TTY-FUNCTIONS-------------------------------------------------
void TTY_DELAY(int cycles){
	HAL_Delay(cycles * ( 1000 / baud));
}

int* TTY_WRITEBUFFER(int* buffer){
    // Safety check for NULL pointer
    if (buffer == NULL) {
        // Return empty buffer
        int* out = (int*)malloc(sizeof(int));
        if (out == NULL) {
            return NULL;  // malloc failed
        }
        out[0] = -1;
        return out;
    }
    
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




void DEB_BLANK(){
	TTY_Startbit();

    TTY_Stopbit(1.0);
}


void DEB_R(){
	setTTY(0);
	HAL_Delay(100);

	setTTY(1);
	HAL_Delay(25);

	setTTY(0);		// Send first Bit (0)
	HAL_Delay(15);	// wait for end of bit

	setTTY(1);		// Send second bit (1)
	HAL_Delay(20);	// Set impulse to 20ms

	setTTY(0);		// Send third bit (0)
	HAL_Delay(20);	// current bit

	setTTY(1);		// Send forth bit (1)
	HAL_Delay(10);	// Set impulse duration to 10ms

	setTTY(0);
	HAL_Delay(10);	// 10ms for rest of forth bit
	HAL_Delay(20);	// Fifth bit

	HAL_Delay(20);	// Stopp-bit
}

void DEB_CR(){
	setTTY(0);
	HAL_Delay(100);

	setTTY(1);
	HAL_Delay(20);

	setTTY(0);		// Send first Bits (000)
	HAL_Delay(60);	// wait for end of bits

	setTTY(1);
	HAL_Delay(10);	// wait for forth bit (1)

	setTTY(0);
	HAL_Delay(10);	// end of bit 4
	HAL_Delay(20);	// Bit 5
	HAL_Delay(20);	// Stopp-bit
}
void DEB_LF(){
	setTTY(0);
	HAL_Delay(100);

	setTTY(1);
	HAL_Delay(20);

	setTTY(0);
	HAL_Delay(20);

	setTTY(1);
	HAL_Delay(10);

	setTTY(0);
	HAL_Delay(90);

}
void TTY_Startbit(){
	setTTY(1);
	HAL_Delay(1000 / baud);
}

void TTY_Stopbit(){
	setTTY(0);
	HAL_Delay((int)(stopbit_cnt * (1000 / baud)));
}
void TTY_WRITE(int _sym){

	if (_sym == symbol.figs || _sym == symbol.ltrs)
		tx_figs = symbol.figs ? 1 : 0;

    // ---TRANSMIT--------------------------------------------------
	TTY_Startbit();

	// LSB FIRST!
    for (int i = 0; i < 5; i++) {
        int bit = (_sym >> i) & 0x01;
        setTTY(bit);
        TTY_DELAY(1);
    }
	// send those 5 bits
	TTY_Stopbit();
}

int readSymbol(){
	// TODO: Implement this thing
	// wait for TTY to SEND sym s to REC
	return 31;
}

void ryLoop(){
	while (1){
		TTY_WRITE(10); 	// send 'r'
		TTY_WRITE(21);	// send 'y'
	}
}

/* this function does multiple things
 * 1. Converts all ASCII-Chars into symbols
 * 2. is sensitive to terminal_width so it appends cr/lf
 * 3. does a space-efficent management of figs/ltrs symbols
*/
int* toSymbols(char* chars){
	// 1. assumption: at PON the machine is in ltrs mode
	//    -> booTY is called
	//int printed_chars = 0; 	// this var keeps track for new-line
	// for each char in string
	for (int i = 0; chars[i] != '\0'; i++){
		// Is char[i] a (valid) Letter?
		if (chars[i] >= 'a' && chars[i] <= 'z'){
			// do letter appending
		}
		else {
			// do symbol stuff
		}
	}
	return NULL;
}


//TODO Delete this sucker
void SEND_TTYC(char c){
	// writes a 7Bit ASCII to CCITT-2
	int out = (int) c;
	TTY_WRITE(out);
}

void setTTY(int state){			// TTY @ A3
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}


// Settings Interface for booTY
void setBaudrate(int baudrate) {baud = baudrate;}
void setTermWidth(int termwidth) {width = termwidth;}
void setStopbits(float stopbit) {stopbit_cnt = stopbit;}
