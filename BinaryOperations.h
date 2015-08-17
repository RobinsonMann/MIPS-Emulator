#ifndef __BINARY_OPERATORS_H__
#define __BINARY_OPERATORS_H__

#include <iostream>

namespace binOp {

	void outputBin (int binary, std::ostream &out);
	int firstSixBits (int instruction);
	int lastFourBytes (int instruction);
	int registerS (int instruction);
	int registerT (int instruction);
	int registerD (int instruction);
	int opcode (int instruction);
}

#endif