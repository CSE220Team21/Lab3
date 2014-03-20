/*	Team 21 (https://github.com/CSE220Team21/Lab3)
 	Mike Mayr (MMayr15), Charlie Crow (LevenBravo)
 	18 Mar 2014
 	CSE 220: T,Th 10:30am
 //  scanner.h
 //  Lab3
 //
 //  Created by Bryce Holton.
 //  Copyright (c) 2014 Bryce Holton. All rights reserved. */

#ifndef Lab3_scanner_h
#define Lab3_scanner_h
#include "common.h"
#include "print.h"

void init_scanner();
Token* get_token();
BOOLEAN get_source_line(char source_buffer[]);

#endif
