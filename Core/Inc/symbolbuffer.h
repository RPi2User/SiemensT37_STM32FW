/*
 * symbolbuffer.h
 *
 *  Created on: Sep 28, 2025
 *      Author: florian
 */

#ifndef INC_SYMBOLBUFFER_H_
#define INC_SYMBOLBUFFER_H_



typedef enum E_lettercase{
	LOWERCASE = 0,
	UPPERCASE = 1
} E_lettercase;

typedef enum E_symbols{
	// Letters
	a = 3,
	b = 25,
	c = 14,
	d = 9,
	e = 1,
	f = 13,
	g = 26,
	h = 20,
	i = 6,
	j = 11,
	k = 15,
	l = 18,
	m = 28,
	n = 12,
	o = 24,
	p = 22,
	q = 23,
	r = 10,
	s = 5,
	t = 16,
	u = 7,
	v = 30,
	x = 29,
	y = 21,
	z = 17,

	// Figures
	n0 = 22,
	n1 = 23,
	n2 = 19,
	n3 = 1,
	n4 = 10,
	n5 = 16,
	n6 = 21,
	n7 = 7,
	n8 = 6,
	n9 = 24,
	period = 28,
	comma = 12,
	plus = 17,
	minus = 3,
	lparen = 15,
	rparen = 18,
	slash = 29,
	colon = 14,
	quote = 5,
	bell = 11,

	// Common
	space = 4,
	cr = 8,
	lf = 2,
	ltrs = 31,
	figs = 27,
	null = 0

} E_symbols;

// --- Basic manipulation ------------------------------------------
uint32_t sbf_len(int8_t* sbf);
int8_t* sbf_createSymbolBuffer(void);

int8_t* sbf_appendSym(int8_t* head, uint8_t sym);
int8_t* sbf_concaternate(int8_t* head, int8_t* tail, uint8_t keepTail);

// --- CONVERT -----------------------------------------------------

char* sbf_convertToString(int8_t* _inSbf);
int8_t* sbf_convertToSymbolBuffer(char* _inStr);

#endif /* INC_SYMBOLBUFFER_H_ */
