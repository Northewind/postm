/* main - interpreter start point and command processing cycle
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

#include <limits.h>
#include "excode.h"
#include "bios.h"
#include "proc.h"


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
	while ((res = runcmd()) != E_HLT) {
		if (res != E_SUCC) return res;
		if (++iter_count == INT_MAX) return E_RUN;
	}
	print_prog();
	print_ram();
	return E_SUCC;
}

