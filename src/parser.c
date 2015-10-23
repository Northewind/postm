/* parser - parsing program file and initial data
 * 
 * Copyright 2015 Alexander Loshkin
 *
 * Post's machine with fixed memory size and inversed check operation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "proc.h"


enum inp_char {
	IC_DIG, IC_PNT, IC_ALP, IC_SEM, IC_SPC
};

enum state {
	S_NUMNAM, S_NUM, S_NUMSPC, S_PNT, S_NAM, S_NAMSPC, S_A1, S_A1SPC, S_SEP, S_SEPSPC, S_A2, S_HALT
};


static int state_table[][5] = {
/*   IC_DIG    IC_PNT    IC_ALP    IC_SEM    IC_SPC
     0-9       .         A-Z       ;         space */
	{S_NUM,    S_HALT,   S_NAM,    S_HALT,   S_NUMNAM}, /* S_NUMNAM -- command number or command name */
	{S_NUM,    S_PNT,    S_HALT,   S_HALT,   S_NUMSPC}, /* S_NUM    -- command number */
	{S_HALT,   S_PNT,    S_HALT,   S_HALT,   S_NUMSPC}, /* S_NUMSPC -- space after command number */
	{S_HALT,   S_HALT,   S_NAM,    S_HALT,   S_PNT},    /* S_PNT    -- point after command number */
	{S_A1,     S_HALT,   S_NAM,    S_HALT,   S_NAMSPC}, /* S_NAM    -- command name */
	{S_A1,     S_HALT,   S_HALT,   S_HALT,   S_NAMSPC}, /* S_NAMSPC -- space after command name */
	{S_A1,     S_HALT,   S_HALT,   S_SEP,    S_A1SPC},  /* S_A1     -- address 1 */
	{S_HALT,   S_HALT,   S_HALT,   S_SEP,    S_A1SPC},  /* S_A1SPC  -- space after address 1 */
	{S_A2,     S_HALT,   S_HALT,   S_HALT,   S_SEPSPC}, /* S_SEP    -- address separator */
	{S_A2,     S_HALT,   S_HALT,   S_HALT,   S_SEPSPC}, /* S_SEPSPC -- space after address separator */
	{S_A2,     S_HALT,   S_HALT,   S_HALT,   S_HALT}    /* S_A2     -- address 2 */
};


static void uncomm(char *s)
{
	if (!s) return;
	while (*s) {
		if (*s == '\r' || *s == '\n' || *s == '#') {
			*s = 0;
			break;
		}
		s++;
	}
}


excode_t prs_progline(char *s)
{
	int a1, a2;
	char *c;
	int state = 0;
	enum inp_char ic;
	opcode_t op;
	a1 = a2 = 0;
	uncomm(s);
	if (!*s) return E_SUCC;

	/*  TO-DO: normal parsing
	c = s;
	while (!c) {
		if (isdigit(*c)) {
			ic = IC_DIG;
		} else if (*c == '.') {
			ic = IC_PNT;
		} else if (isalpha(*c)) {
			ic = IC_ALP;
		} else if (*c == ';') {
			ic = IC_SEM;
		} else if (isspace(*c)) {
			ic = IC_SPC;
		} else {
			return E_PARSE;
		}
		state = state_table[state][ic];
		printf("state: %d\n", state);
		c++;
	}
	*/



	switch (s[0]) {
	case 'V':
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		op = OP_SET;
		break;
	case 'X': 
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		op = OP_UNSET;
		break;
	case 'L':
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		op = OP_LFT;
		break;
	case 'R':
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		op = OP_RGH;
		break;
	case '?':
		if (sscanf(s+1, "%d;%d", &a1, &a2) != 2) return E_PARSE;
		op = OP_CHK;
		break;
	case '!':
		op = OP_HLT;
		break;
	case '\n': /* Ignore whitespace */
	case '\r':
	case '\t':
	case ' ':
		break;
	default:
		return E_PARSE;
	}
	if (a1 < 0 || a2 < 0) return E_PARSE;
	return cmd_add(op, a1, a2);
}


excode_t prs_datafile(FILE *f)
{
	int addr = 0;
	int c;
	while ((c = fgetc(f)) != EOF) {
		switch(c) {
		case '0':
			ram[addr++] = 0;
			break;
		case '1':
			ram[addr++] = 1;
			break;
		case '\n': /* Ignore whitespace */
		case '\r':
		case '\t':
		case ' ':
			break;	
		default:
			return E_HD_PRS;
		}
		if (addr == RAM_SIZE) break;
	}
	if (ferror(f)) return E_FILE;
	memset(ram + addr, 0, RAM_SIZE - addr);
	return E_SUCC; 
}

