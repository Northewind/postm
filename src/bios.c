/* bios - functions for IO operations
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
#include <stdlib.h>
#include <string.h>
#include "bios.h"

#define FIL_ROM "rom.txt"
#define FIL_HD  "hd.txt"
#define CMD_STR_LEN 64 
#define PROG_SIZE_INCR 32

cmd_t *prog = NULL;
int prog_len = 0;
char ram[RAM_SIZE];

static FILE *fil_hd = NULL;


static excode_t cmd_add(opcode_t op, addr_t a1, addr_t a2)
{
	cmd_t *p;
	static int reserved = 0;
	if (prog_len == reserved) {
		if (!(p = realloc(prog, CMD_SIZE*(reserved + PROG_SIZE_INCR)))) {
			return E_MEM;
		}
		prog = p;
		reserved += PROG_SIZE_INCR;
	}
	prog[prog_len].op = op;
	prog[prog_len].a1 = a1 - 1;
	prog[prog_len].a2 = a2 - 1;
	prog_len++;
	return E_SUCC;
}


excode_t read_rom()
{
	char *cmd;
	FILE *f;
	int addr1, addr2;
	excode_t result = E_SUCC;
	if ((cmd = malloc(CMD_STR_LEN)) == NULL) return E_MEM;
	if ((f = fopen(FIL_ROM, "r")) == NULL) {
		free(cmd);
		return E_ROM;
	}
	while (fgets(cmd, CMD_STR_LEN, f)) {
		switch (cmd[0]) {
		case 'V':
			if (sscanf(cmd+1, "%d", &addr1) != 1) goto prs;
			if (cmd_add(OP_SET, addr1, 0) != E_SUCC) goto mem;
			break;
		case 'X': 
			if (sscanf(cmd+1, "%d", &addr1) != 1) goto prs;
			if (cmd_add(OP_UNSET, addr1, 0) != E_SUCC) goto mem;
			break;
		case 'L':
			if (sscanf(cmd+1, "%d", &addr1) != 1) goto prs;
			if (cmd_add(OP_LFT, addr1, 0) != E_SUCC) goto mem;
			break;
		case 'R':
			if (sscanf(cmd+1, "%d", &addr1) != 1) goto prs;
			if (cmd_add(OP_RGH, addr1, 0) != E_SUCC) goto mem;
			break;
		case '?':
			if (sscanf(cmd+1, "%d;%d", &addr1, &addr2) != 2) goto prs;
			if (cmd_add(OP_CHK, addr1, addr2) != E_SUCC) goto mem;
			break;
		case '!':
			if (cmd_add(OP_HLT, 0, 0) != E_SUCC) goto mem;
			break;
		case '#':  /* For comments */
		case '\n': /* Ignore whitespaces */
		case '\r':
		case ' ':
			break;
		default:
			goto prs;
		}
	}
	if (ferror(f)) result = E_ROM; 
cln: /* Clean and return */
	fclose(f);
	free(cmd);
	free(prog);
	return result;
prs: /* Parse error */
	result = E_PARSE;
	goto cln;
mem: /* Memory error */
	result = E_MEM;
	goto cln;
}


excode_t read_hd()
{
	int ad = 0;
	int c;
	if ((fil_hd = fopen(FIL_HD, "r+")) == NULL) return E_HD;
	while ((c = fgetc(fil_hd)) != EOF) {
		switch(c) {
		case '0':
			ram[ad++] = 0;
			break;
		case '1':
			ram[ad++] = 1;
			break;
		case '\n': /* Just ignore */
		case '\r':
		case ' ':
			break;	
		default:
			return E_HD_PRS;
		}
		if (ad == RAM_SIZE) break;
	}
	if (ferror(fil_hd)) return E_HD;
	memset(ram + ad, 0, RAM_SIZE - ad);
	return E_SUCC;
}


excode_t store_ram(FILE *f)
{
	int ad;
	if (!f) f = fil_hd;
	for (ad = 0; ad < RAM_SIZE; ad++) {
		if (ad % 128 == 0) fputc('\n', f);
		if (ad % 8 == 0) fputc(' ', f);
		switch (ram[ad]) {
			case 0:
				if (fputc('0', f) == EOF) return E_HD;
				break;
			case 1:
				if (fputc('1', f) == EOF) return E_HD;
				break;
		}
	}
	return E_SUCC;
}


excode_t fini_hd()
{
	excode_t r = E_SUCC;
	if (fclose(fil_hd) == EOF)
		r = E_HD;
	else
		fil_hd = NULL;
	return r;
}


void print_cmd(cmd_t c)
{
	c.a1++;
	c.a2++;
	switch (c.op) {
	case OP_SET:
		printf("V%d\n", c.a1);
		break;
	case OP_UNSET:
		printf("X%d\n", c.a1);
		break;
	case OP_LFT:
		printf("L%d\n", c.a1);
		break;
	case OP_RGH:
		printf("R%d\n", c.a1);
		break;
	case OP_CHK:
		printf("?%d;%d\n", c.a1, c.a2);
		break;
	case OP_HLT:
		printf("!\n");
		break;
	}
}


void print_prog()
{
	int pc;
	for (pc = 0; pc < prog_len; pc++) {
		printf("%d: ", pc+1);
		print_cmd(prog[pc]);
	}
}

