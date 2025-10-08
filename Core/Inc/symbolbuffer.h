/*
 * symbolbuffer.h
 *
 *  Created on: Sep 28, 2025
 *      Author: florian
 */

#ifndef INC_SYMBOLBUFFER_H_
#define INC_SYMBOLBUFFER_H_

extern const int8_t SBF_TERMINATOR;


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
	w = 19,
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
	equals = 30,
	question = 25,

	// Common
	space = 4,
	cr = 8,
	lf = 2,
	ltrs = 31,
	figs = 27,
	null = 0,

	// Special symbols, begins @ 0x50 === 80d
	enq	= 0x55,	// werda?
	tab = 0x59,
	ff	= 0x5C,	// form feed -> 10x new line



} E_symbols;


// Conversion LUTs: ------------------------------------------------

static const char ltrs_to_char[32] = {
	[a] = 'a',
	[b] = 'b',
	[c] = 'c',
	[d] = 'd',
	[e] = 'e',
	[f] = 'f',
	[g] = 'g',
	[h] = 'h',
	[i] = 'i',
	[j] = 'j',
	[k] = 'k',
	[l] = 'l',
	[m] = 'm',
	[n] = 'n',
	[o] = 'o',
	[p] = 'p',
	[q] = 'q',
	[r] = 'r',
	[s] = 's',
	[t] = 't',
	[u] = 'u',
	[v] = 'v',
	[w] = 'w',
	[x] = 'x',
	[y] = 'y',
	[z] = 'z'
};

static const char figs_to_char[32] = {
	[n0] = '0',
	[n1] = '1',
	[n2] = '2',
	[n3] = '3',
	[n4] = '4',
	[n5] = '5',
	[n6] = '6',
	[n7] = '7',
	[n8] = '8',
	[n9] = '9',
	[period] = '.',
	[comma] = ',',
	[plus] = '+',
	[minus] = '-',
	[lparen] = '(',
	[rparen] = ')',
	[slash] = '/',
	[colon] = ':',
	[quote] = 0x27,
	[bell] = 0x07,
	[equals] = '=',
	[question] = '?'
};

static const int8_t char_to_symCommon[128] = {
	[0x03]	=	null, // strg+c
	[0x04] 	=	null, // strg+d
	[0x05]	=	enq,
	[0x07]	=	bell,
	[0x09]	=	tab,
	[0x0A]	=	lf,
	[0x0C]	=	ff,
	[0x0D]	= 	cr,
	[0x20]	= 	space
};

static const int8_t char_to_symFIGS[128] = {
	[0x07]	=	bell,
	[0x27]	=	quote,
	['{']	=	lparen,
	['}']	=	rparen,
	['<']	=	quote,
	['>']	=	quote,
	['[']	=	lparen,
	[']']	=	rparen,
	['(']	=	lparen,
	[')']	=	rparen,
	['+']	=	plus,
	[',']	=	comma,
	['-']	=	minus,
	['.']	=	period,
	['/']	=	slash,
	['0']	=	n0,
	['1']	=	n1,
	['2']	=	n2,
	['3']	=	n3,
	['4']	=	n4,
	['5']	=	n5,
	['6']	=	n6,
	['7']	=	n7,
	['8']	=	n8,
	['9']	=	n9,
	[':']	=	colon,
	['=']	=	equals,
	['?']	=	question
};

static const int8_t char_to_symLTRS[128] = {
	['A']	=	a,
	['B']	=	b,
	['C']	=	c,
	['D']	=	d,
	['E']	=	e,
	['F']	=	f,
	['G']	=	g,
	['H']	=	h,
	['I']	=	i,
	['J']	=	j,
	['K']	=	k,
	['L']	=	l,
	['M']	=	m,
	['N']	=	n,
	['O']	=	o,
	['P']	=	p,
	['Q']	=	q,
	['R']	=	r,
	['S']	=	s,
	['T']	=	t,
	['U']	=	u,
	['V']	=	v,
	['W']	=	w,
	['X']	=	x,
	['Y']	=	y,
	['Z']	=	z
};

// -----------------------------------------------------------------

// --- Basic manipulation ------------------------------------------
uint32_t sbf_len(int8_t* sbf);
int8_t* sbf_createSymbolBuffer(void);

int8_t* sbf_appendSym(int8_t* head, uint8_t sym);
int8_t* sbf_concaternate(int8_t* head, int8_t* tail, uint8_t keepTail);

// --- CONVERT -----------------------------------------------------

char* sbf_convertToString(int8_t* _inSbf,
		char* _newLine, uint8_t keepBuffer);
int8_t* sbf_convertToSymbolBuffer(char* _inStr);

#endif /* INC_SYMBOLBUFFER_H_ */
