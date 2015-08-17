#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <iostream>
#include "Memory.h"
#include "BinaryOperations.h"
#include "StringUtils.h"

class Machine {

	const static int outputAddress = 0xFFFF000C; // Send a word to stdout
	const static int inputAddress  = 0xFFFF0004; // Load a word from stdin
	const static int returnAddress = 0xFFFF0000; // Return to machine

	Memory memory;

	int registers [32];
	int high; 
	int low;
	int pc;

public:

	/* Constructor */
	Machine (int size);

	/* Load a MIPS program into the machine*/
	void loadProgram(std::istream &in);

	void setRegister(int reg, int value);
	int getRegister(int reg);

	void setRegisterUnsigned(int reg, unsigned int value);
	unsigned int getRegisterUnsigned(int reg);

	/* Starts execution of the machine */
	void execute(bool);
};

#endif