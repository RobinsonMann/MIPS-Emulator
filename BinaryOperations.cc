#include "BinaryOperations.h"

namespace binOp {

	void outputBin (int binary, std::ostream &out) {
		out << (char) (binary >> 24) << (char) (binary >> 16) << (char) (binary >> 8) << (char) (binary);
	}

	int firstSixBits (int instruction) {
		return instruction & 0xFC000000;
	}

	int lastFourBytes (int instruction) {
		return (short) instruction & 0x0000FFFF;
	}

	int registerS (int instruction) {
		return (instruction & 0x03E00000) >> 21;
	}

	int registerT (int instruction) {
		return (instruction & 0x001F0000) >> 16;
	}

	int registerD (int instruction) {
		return (instruction & 0x0000F800) >> 11;
	}

	int opcode (int instruction) {
		return instruction & 0x000007FF;
	}
}