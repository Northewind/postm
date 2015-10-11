/* main - start point and main instruction processor
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
#include <limits.h>
#include "main.h"
#include "bios.h"

static int PC = 0;
static int addr = 0;


static excode_t runcmd()
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
	if (PC < 0 || PC >= prog_len) return E_RUNERR;
	return E_SUCC;
}


int main(int argc, char* argv[])
{
	excode_t res;
	int iter_count = 0;
	if (argc != 3) {
		print_usage();
		return E_START;
	}
	if ((res = read_rom(argv[1])) != E_SUCC) return res;
	if ((res = read_hd(argv[2])) != E_SUCC) return res;
	print_prog(); /* Debug print */
	while ((res = runcmd()) != E_HLT) {
		if (res != E_SUCC) return res;
		if (++iter_count == INT_MAX) return E_RUNERR;
	}
	if ((res = store_ram(stdout)) != E_SUCC) return res;
	return fini_hd();
}

