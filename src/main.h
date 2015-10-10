/* main - main data types
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

#ifndef MAIN_H
#define MAIN_H

typedef enum excode {
	E_SUCC, E_HLT, E_ROM, E_MEM, E_PARSE, E_HD, E_HD_PRS, E_RUNERR
} excode_t;


typedef enum opcode {
	OP_SET, OP_UNSET, OP_LFT, OP_RGH, OP_CHK, OP_HLT
} opcode_t;


typedef int addr_t;


typedef struct {
	opcode_t op;
	addr_t a1;
	addr_t a2;
} cmd_t;

#define CMD_SIZE sizeof(cmd_t)

#endif /* MAIN_H */
