#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <iostream>

#include "BinaryOperations.h"
#include "StringUtils.h"

class Memory {

	// Size of the program loaded into memory
	int programSize;

	// Total size of memory
	int size;

	// Array of memory. Cannot be an int[] as size determined at runtime
	int *memory;

	public:

	// Constructor 
	Memory(int size);

	// Destructor 
	~Memory();

	// Loads a program into memory
	void loadProgram (std::istream &in);

	// [] Index operator 
	int operator [] (int i) const;

	// [] Assignment operator
    int &operator [] (int i);
};


#endif