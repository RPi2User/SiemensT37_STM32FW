#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include "symbolbuffer.h"
#include "tty.h"

// --- DATA BLOCKS -------------------------------------------------
// Hardware Variables
float baudrate = 50.0;		// default values
uint32_t lineWidth = 70;
E_lettercase lettercase = LOWERCASE;
float stopbit_cnt = 1.5;
uint8_t tabSize = 2;


// System variables
uint8_t emerg_cnt = 0;		// Emergency Counter, reserved on stack
tty_mode_t tty_mode = TTY_LETTERS;
uint8_t loopback = 0;		// This sends bit right back to TTY

uint8_t READ_TIMEOUT = 100;	// Timeout of 1000ms
char* fox = "\r\nthe quick brown fox jumps over the lazy dog";

const int8_t SBF_MEM_ERROR[] = {
		cr, lf, ltrs, m, e, m, o, r, y, space, e, r, r, o, r, space,
		r, e, s, e, t, t, i, n, g, space, c, p, u, figs, bell, bell,
		-1
};


void TTY_Init(){
	// this loads the contents from flash and sets the variables
	return NULL;
}

// -----------------------------------------------------------------
// Debug function prints a brown fox
void TTY_Fox(void){
	TTY_WriteString(fox, 1);
}


// ---TTY-FUNCTIONS-------------------------------------------------
void TTY_WriteKey(char key){
	//TODO
}


void TTY_WriteString(char* str, uint8_t keepStr){
    // MAIN WRITE FUNCTION
    // keepStr is used when you don't want to clear a constant
    int8_t* writebuffer = malloc(0);
    writebuffer = sbf_convertToSymbolBuffer(str);
    writebuffer = TTY_WriteBuffer(writebuffer);
    if (keepStr == 0)
    	free(str);
}

int8_t* TTY_WriteBuffer(int8_t* buffer){
    
    // Write all symbols in buffer
    for (uint8_t i = 0; buffer[i] != -1; i++) {
        TTY_Write(buffer[i]);
    }

    // Free the input buffer
    free(buffer);
    
    // Create new empty buffer
    int8_t* out = (int8_t*)malloc(sizeof(int8_t));
    if (out == NULL) {
        return NULL;  // malloc failed - caller must handle this!
    }
    out[0] = -1;  // Null-terminate
    return out;
}

void TTY_Write(int8_t _sym){
	setLED_BSY(1);
	if (_sym == -1) return;

	// Skip redundant ltrs/figs commands
	if (_sym == TTY_FIGURES || _sym == TTY_LETTERS)
		tty_mode = TTY_FIGURES ?
				TTY_FIGURES : TTY_LETTERS;

    // ---TRANSMIT--------------------------------------------------
	TTY_Startbit();

	// LSB FIRST!
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t bit = ((_sym >> i) & 0x01) ^ 1;
        setTTY(bit);
        TTY_Delay(1);
    }
	// send those 5 bits
	TTY_Stopbit();
	setLED_BSY(0);
}

/*	==== READ OPERATIONS ===========================================
 *
 * This section contains all functions regarding reading Data from
 * the Teletype. I tried to keep this section clean but some stuff
 * is quite necessary for a stable "API" like infrastructure.
 *
 *	- TTY_ReadKey() is the main function, this reads a single Symbol
 *				    and converts it into a char
 */
char TTY_ReadKey(){
	// TODO
	return 'C';
}

int8_t readSymbol() {
	// wait for Symbol-Trigger
	while(1){
		if (readTTY() == 0) TTY_DelayMS(2);
		else break;
	}
	// read start-bit
	// pattern: 20ms startbit, 5x20ms Databit, 1.n stopbits
	// STARTBIT is 20ms HIGH
	TTY_DelayMS(10);	// Wait 10ms
	uint8_t beg = readTTY();
	TTY_DelayMS(15);	// Wait 5 + 10ms
	// NOTE: the additional delay (above) is the SAMPLING OFFSET!

	Databit databit[5];
	for (uint8_t i = 0; i < 5; i++){
		databit[i].s1 = readTTY();
		TTY_DelayMS(5); // 5ms Delay
		databit[i].s2 = readTTY();
		TTY_DelayMS(5); // 5ms Delay
		databit[i].s3 = readTTY();
		TTY_DelayMS(10); // 10ms delay
	}

	TTY_DelayMS(5);


	uint8_t end = readTTY();

	// Eval Bits
	if (beg == 0 || end != 0){
		setReadError();
		return -1;
	}

    uint8_t out = 0;

    if (majority(databit[0]) == 0) out += 1;
    if (majority(databit[1]) == 0) out += 2;
    if (majority(databit[2]) == 0) out += 4;
    if (majority(databit[3]) == 0) out += 8;
    if (majority(databit[4]) == 0) out += 16;

    clearReadError();

    HAL_Delay(10);	// Stobbit should be <20ms
    return out;
}

uint8_t majority(Databit d) {
    return (d.s1 + d.s2 + d.s3) >= 2 ? 1 : 0;
}

// --- Hardware Interfaces -----------------------------------------
int8_t readTTY(){
	int8_t out = -1;
	out = HAL_GPIO_ReadPin(GPIOB, TTY_RECV_Pin);
	if (loopback != 0) setTTY(out);
	return out;
}

void setTTY(uint8_t state){			// TTY @ A3
	if (state != 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}

void TTY_DelayMS(uint32_t ms){
	uint32_t start = HAL_GetTick();
	while ((HAL_GetTick() - start) < ms);
}

void TTY_Delay(float cycles){
	uint32_t delay_ms = (cycles * ( 1000 / baudrate));
	TTY_DelayMS(delay_ms);

}

void TTY_Startbit(){
	setTTY(1);
	TTY_Delay(1.0);
}

void TTY_Stopbit(){
	setTTY(0);
	TTY_Delay(stopbit_cnt);
}

void TTY_raiseMemoryError(void){
	/* Oh no. you managed to see this :c
	 * I'm very sorry for seeing that you got a Memory error.
	 * This function get Called when your Heap is full/corrupted
	 *
	 * 1. Send SBF_MEM_ERROR[], symbol by symbol
	 * 2. Reset CPU
	 * 3. Hope for the best
	 */
	emerg_cnt = 0;
	while(SBF_MEM_ERROR[emerg_cnt] != SBF_TERMINATOR){
		TTY_Write(SBF_MEM_ERROR[emerg_cnt]);
	}
	NVIC_SystemReset();	// REBOOT CPU
}

void setReadError(){ HAL_GPIO_WritePin(GPIOA, TTY_READERR_Pin, 1); }
void clearReadError(){ HAL_GPIO_WritePin(GPIOA, TTY_READERR_Pin, 0); }

// --- System Properties -------------------------------------------
void setLoopback(uint8_t _loopback) {loopback = _loopback;}
void setBaudrate(float baudrate) {baudrate = baudrate;}
void setTermWidth(uint8_t termwidth) {lineWidth = termwidth;}
void setStopbits(float stopbit) {stopbit_cnt = stopbit;}
// TODO add Lower/Uppercase "switch"
