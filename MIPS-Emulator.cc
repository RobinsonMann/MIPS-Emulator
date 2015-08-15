#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <cstdlib> 
#include <sstream>

int stoi(const std::string& strInt) {
    std::istringstream ss(strInt);
    int i;
    ss >> i;
    return i;
}

namespace binOp {

	void outputBin (int binary, std::ostream &out) {
		out << (char) (binary >> 24) << (char) (binary >> 16) << (char) (binary >> 8) << (char) (binary);
	}

	int firstSixBytes (int instruction) {
		return instruction & 0xFC000000;
	}

	int lastFourBytes (int instruction) {
		return instruction & 0x0000FFFF;
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

class Memory {

	int programSize;
	int size;
	int *memory;

public:

	/* Constructor */
	Memory(int size) : size(size), memory(new int[size]), programSize(0) { for(int i=0;i<size;i++)memory[i]=0; }

	/* Destructor */
	~Memory() { delete memory; }

	/* Loads a program into memory*/
	void loadProgram (std::istream &in) {
	 	for (int i = 0; !in.eof(); i++, programSize++) {
			memory[i] = (in.get() << 24) | (in.get() << 16) | (in.get() << 8) | (in.get());	
		}
		programSize--; // Remove EOF
	}

	/* Access an address of memory */
	int operator [] (int i) { return memory[i]; }

};

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
	Machine (int size) : memory(Memory(size)), pc(0) {
		registers[31] = returnAddress; // Return addresss
		registers[30] = size * 4;      // Last address in memory
	}

	/* Load a MIPS program into the machine*/
	void loadProgram(std::istream &in) { memory.loadProgram(in); }

	void setRegister(int reg, int value) {
		if      (reg <  0) std::cerr << "Tried to set register < 0"  << std::endl;
		else if (reg > 31) std::cerr << "Tried to set register > 31" << std::endl;
		else               registers[reg] = value;
	}

	int getRegister(int reg) {
		if      (reg <  0) std::cerr << "Tried to access register < 0"  << std::endl;
		else if (reg > 31) std::cerr << "Tried to access register > 31" << std::endl;
		else               return registers[reg];
	}
 
 	void setRegisterUnsigned(int reg, unsigned int value) {
		if      (reg <  0) std::cerr << "Tried to set register < 0"  << std::endl;
		else if (reg > 31) std::cerr << "Tried to set register > 31" << std::endl;
		else               registers[reg] = value;
	}

	unsigned int getRegisterUnsigned(int reg) {
		if      (reg <  0) std::cerr << "Tried to access register < 0"  << std::endl;
		else if (reg > 31) std::cerr << "Tried to access register > 31" << std::endl;
		else               return registers[reg];
	}
	/* Starts execution of the machine */
	void execute() {

		while (true) {

			if (binOp::firstSixBytes(memory[pc]) == 0x00000000) {

				/* ADD, SUB, MULT, DIV, MFHI, MFLO, LIS, SLT, JR, JALR */
				if (binOp::opcode(memory[pc]) == 0x00000020) {        // ADD
					setRegister(binOp::registerD(memory[pc]), getRegister(binOp::registerS(memory[pc])) + getRegister(binOp::registerT(memory[pc])));
				} else if (binOp::opcode(memory[pc]) == 0x00000022) { // SUB
					setRegister(binOp::registerD(memory[pc]), getRegister(binOp::registerS(memory[pc])) - getRegister(binOp::registerT(memory[pc])));
				} else if (binOp::opcode(memory[pc]) == 0x00000018) { // MULT
				} else if (binOp::opcode(memory[pc]) == 0x00000019) { // MULTU
				} else if (binOp::opcode(memory[pc]) == 0x0000001a) { // DIV
				} else if (binOp::opcode(memory[pc]) == 0x0000001b) { // DIVU
				} else if (binOp::opcode(memory[pc]) == 0x00000010) { // MFHI
				} else if (binOp::opcode(memory[pc]) == 0x00000012) { // MFLO
				} else if (binOp::opcode(memory[pc]) == 0x00000014) { // LIS
				} else if (binOp::opcode(memory[pc]) == 0x0000002a) { // SLT
				} else if (binOp::opcode(memory[pc]) == 0x0000002b) { // SLTU
				} else if (binOp::opcode(memory[pc]) == 0x00000008) { // JR
					pc = getRegister(binOp::registerS(memory[pc]));
					if (pc == returnAddress) break;					
				} else if (binOp::opcode(memory[pc]) == 0x00000009) { // JALR
					int temp = getRegister(binOp::registerS(memory[pc]));
					registers[31] = pc;
					pc = temp;
					if (pc == returnAddress) break;					
				}

			} else if (binOp::firstSixBytes(memory[pc]) == 0x8C000000) {
				/* LW */
			} else if (binOp::firstSixBytes(memory[pc]) == 0xAC000000) {
				/* SW */
			} else if (binOp::firstSixBytes(memory[pc]) == 0x10000000) {
				/* BEQ */
			} else if (binOp::firstSixBytes(memory[pc]) == 0x14000000) {
				/* BNE */
			} else {
				std::cerr << "Invalid opcode" << std::endl;
				break;
			}

			pc++;
		}

	}
};

int main (int argc, char *argv[]) {

	std::ifstream input ((std::string) argv[1]);

	Machine machine = Machine(65536);
	machine.setRegister(1, stoi(argv[2]));
	machine.setRegister(2, stoi(argv[3]));

	machine.loadProgram(input);
	machine.execute();

	std::cout << "Register $3: " << machine.getRegister(3) << std::endl;

	return 0;
}