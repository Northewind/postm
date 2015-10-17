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
#include "parser.h"

#define CMD_LINE_SIZE 128

excode_t read_rom(char *filename)
{
	char cmd[CMD_LINE_SIZE];
	FILE *f;
	int lineno;
	excode_t res = E_SUCC;
	if ((f = fopen(filename, "r")) == NULL) return E_FILE;
	for (lineno = 1;  fgets(cmd, CMD_LINE_SIZE, f);  lineno++) {
		if ((res = prs_progline(cmd)) == E_PARSE) {
			fprintf(stderr, "Parse error in line #%d:\n\n"
	                        "\t%s\n\n", lineno, cmd);
			break;
		} else if (res != E_SUCC) {
			break;
		}
	}
	res = (ferror(f)) ? E_FILE : res;
	fclose(f);
	return res;
}


excode_t read_hd(char* filename)
{
	excode_t res;
	FILE *f;
	if ((f = fopen(filename, "r")) == NULL) return E_FILE;
	res = prs_datafile(f);
	fclose(f);
	return res;
}


void print_ram()
{
	int addr;
	for (addr = 0; addr < RAM_SIZE; addr++) {
		if (addr % 128 == 0) putchar('\n');
		if (addr % 8 == 0) putchar(' ');
		switch (ram[addr]) {
		case 0:
			putchar('0');
			break;
		case 1:
			putchar('1');
			break;
		}
	}
	putchar('\n');
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
	int n = prog_len();
	for (pc = 0; pc < n; pc++) {
		printf("%d. ", pc+1);
		print_cmd(prog[pc]);
	}
}

void print_usage()
{
	puts("Usage: postm <progfile> <memfile>");
}
