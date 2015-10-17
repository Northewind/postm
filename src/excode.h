/* excode - exit codes that program return
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

#ifndef EXCODE_H
#define EXCODE_H

typedef enum excode {
	E_SUCC,   /* Operation successfully completes */
	E_HLT,    /* Stop of runtime (successfully completing) */
	E_FILE,   /* Common file IO error */
	E_START,  /* Invalid command line args */
	E_MEM,    /* Not enough memory */
	E_PARSE,  /* Parse error in program text */
	E_HD_PRS, /* Parse error in memory file */
	E_RUN     /* Runtime error: jump to non-existent command, cycle iters exceeded */
} excode_t;

#endif /* EXCODE_H */
