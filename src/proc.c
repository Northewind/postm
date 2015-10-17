/* processor - main instruction processor
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

#include <stdlib.h>
#include "proc.h"

#define PROG_SIZE_INCR 32

cmd_t *prog = NULL;
char ram[RAM_SIZE];

static int cmd_count = 0;
static int PC = 0;
static int addr = 0;


int prog_len()
{
	return cmd_count;
}


excode_t cmd_add(opcode_t op, addr_t a1, addr_t a2)
{
	cmd_t *p;
	static int reserved = 0;
	if (cmd_count == reserved) {
		if (!(p = realloc(prog, CMD_SIZE*(reserved + PROG_SIZE_INCR)))) {
			return E_MEM;
		}
		prog = p;
		reserved += PROG_SIZE_INCR;
	}
	prog[cmd_count].op = op;
	prog[cmd_count].a1 = a1 - 1;
	prog[cmd_count].a2 = a2 - 1;
	cmd_count++;
	return E_SUCC;
}


excode_t runcmd()
{
	switch (prog[PC].op) {
	case OP_SET:
		ram[addr] = 1;
		PC = prog[PC].a1;
		break;
	case OP_UNSET:
		ram[addr] = 0;
		PC = prog[PC].a1;
		break;
	case OP_LFT:
		addr = (addr == 0) ? RAM_SIZE-1 : addr-1;
		PC = prog[PC].a1;
		break;
	case OP_RGH:
		addr = (addr + 1) % RAM_SIZE;
		PC = prog[PC].a1;
		break;
	case OP_CHK:
		PC = (ram[addr]) ? prog[PC].a1 : prog[PC].a2;
		break;
	case OP_HLT:
		return E_HLT;
	}
	if (PC < 0 || PC >= cmd_count) return E_RUN;
	return E_SUCC;
}

