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
static int iter_count = 0;


static excode_t run()
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
		if (ram[addr])
			PC = prog[PC].a1;
		else
			PC = prog[PC].a2;
		break;
	case OP_HLT:
		return E_HLT;
	}
	if (PC < 0 || PC >= prog_len || ++iter_count == INT_MAX)
		return E_RUNERR;
	else
		return E_SUCC;
}


int main(int argc, char* argv[])
{
	excode_t res;
	if ((res = read_rom()) != E_SUCC) return res;
	if ((res = read_hd()) != E_SUCC) return res;
	while ((res = run()) != E_HLT) {
		if (res != E_SUCC) return res;
	}
	res = store_ram(NULL);
	res = fini_hd();
	return res;
}

