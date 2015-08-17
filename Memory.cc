#include "Memory.h"

/* Constructor e*/
Memory::Memory(int size) : size(size) { 
	
	memory = new int[size];
	programSize = 0;

	for(int i = 0; i < size; i++) 
	{
		memory[i] = 0; 
	}

}

/* Destructor */
Memory::~Memory() {
	delete memory;
}

/* Loads a program into memory */
void Memory::loadProgram (std::istream &in) {

 	for (int i = 0; !in.eof(); i++, programSize++)
 	{
		memory[i] = (in.get() << 24) | (in.get() << 16) | (in.get() << 8) | (in.get());	
	}

	programSize--; // Remove EOF token in memory
}

/* [] Index operator */
int Memory::operator [] (int i) const { 
	return memory[i];
}

/* [] Assignemnt operator */
int & Memory::operator [] (int i) {
	return memory[i];
}
