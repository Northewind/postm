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
#include "parser.h"
#include "proc.h"

excode_t prs_progline(char *s)
{
	excode_t res = E_SUCC;
	int a1, a2;
	a1 = a2 = 0;
	switch (s[0]) {
	case 'V':
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		res = cmd_add(OP_SET, a1, a2);
		break;
	case 'X': 
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		res = cmd_add(OP_UNSET, a1, a2);
		break;
	case 'L':
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		res = cmd_add(OP_LFT, a1, a2);
		break;
	case 'R':
		if (sscanf(s+1, "%d", &a1) != 1) return E_PARSE;
		res = cmd_add(OP_RGH, a1, a2);
		break;
	case '?':
		if (sscanf(s+1, "%d;%d", &a1, &a2) != 2) return E_PARSE;
		res = cmd_add(OP_CHK, a1, a2);
		break;
	case '!':
		res = cmd_add(OP_HLT, a1, a2);
		break;
	case '\n': /* Ignore whitespace */
	case '\r':
	case ' ':
		break;
	default:
		return E_PARSE;
	}
	if (a1 < 0 || a2 < 0) res = E_PARSE;
	return res;
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

