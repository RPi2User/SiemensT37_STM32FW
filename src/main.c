#include <stdio.h>
#include <stdlib.h>

/* This project is the main source for the STM32 Firmware
 * This source has 3 Main objectives
 *  1. PRINT STUFF VIA GPIO IN CCITT-2 STD
 *  2. Get Data from USB (via 9600N1 Serial)
 *  3. Get Data from WiFi (via ESP8266)
 *
 */


// --- PIN DEFINITIONS ---------------------------------------------
// over here there are the functions to toggle those pescy pins
// ignore the integers pls…


// TTY
int ttySend = 0;
int ttyReceive = 0;


// LEDs
int LED_MLOCAL = 0;
int LED_MSERIAL = 0;
int LED_ERROR = 0;


// ESP8266, UART 9600N1
int WiFi1 = 0;
int WiFi2 = 0;

// Button
int Button = 0; 


// -----------------------------------------------------------------

// --- GOBLAL VARIABLES --------------------------------------------

char* writeBuffer;	// Create empty writeBuffer

// Teletype Variables
int rx_figs = 0; 	// wether or not currently in figs or ltrs mode
int tx_figs = 0; 	// ebd.


// Mode-specific vars
int mode = 0; // 0 -> Local != 0 -> Serial


// -----------------------------------------------------------------
void setMode(){
}

int getMode(){
	return 0;
}

void manageIO(){
	setMode(); 			// Set LEDs according to current mode
	mode = getMode();	// Get Mode from button
}

void _mode(){
}

int main()
{
	writeBuffer = malloc(0);

	while(1){
		manageIO();	// Like toggle LEDs, poll Button, etc.
		_mode();
		// do smth important
		// 0. poll teletype
        // 1. poll RS232 Port
		// 2. signalize ESP8266 for transmit
	}
	return 0;
}
