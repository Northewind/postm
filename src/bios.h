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

#ifndef BIOS_H
#define BIOS_H

#include <stdio.h>
#include "excode.h"
#include "proc.h"

excode_t read_rom(char* filename);
excode_t read_hd(char* filename);

void print_ram();
void print_cmd(cmd_t);
void print_prog();
void print_usage();

#endif /* BIOS_H */
