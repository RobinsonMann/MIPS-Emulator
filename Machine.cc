#include "Machine.h"


/* Constructor */
Machine::Machine (int size) : memory(Memory(size)), pc(0) {
	registers[31] = returnAddress; // Return addresss
	registers[30] = size * 4;      // Last address in memory
	for (int i = 0; i < 30; i++) registers[i] = 0;
}

/* Load a MIPS program into the machine*/
void Machine::loadProgram(std::istream &in) { memory.loadProgram(in); }

void Machine::setRegister(int reg, int value) {
	if      (reg <  1) std::cerr << "Tried to set register < 1"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to set register > 31" << std::endl;
	else               registers[reg] = value;
}

int Machine::getRegister(int reg) {
	if      (reg <  0) std::cerr << "Tried to access register < 0"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to access register > 31" << std::endl;
	else               return registers[reg];
}

	void Machine::setRegisterUnsigned(int reg, unsigned int value) {
	if      (reg <  1) std::cerr << "Tried to set register < 1"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to set register > 31" << std::endl;
	else               registers[reg] = value;
}

unsigned int Machine::getRegisterUnsigned(int reg) {
	if      (reg <  0) std::cerr << "Tried to access register < 0"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to access register > 31" << std::endl;
	else               return registers[reg];
}

/* Starts execution of the machine */
void Machine::execute() {

	while (true)
	{

		if (binOp::firstSixBits(memory[pc]) == 0x00000000)
		{
			/* Possible instructions: ADD, SUB, MULT, DIV, MFHI, MFLO, LIS, SLT, JR, JALR */

			if (binOp::opcode(memory[pc]) == 0x00000020)
			{ // ADD -- Add
				setRegister(binOp::registerD(memory[pc]), getRegister(binOp::registerS(memory[pc])) + getRegister(binOp::registerT(memory[pc])));
			}
			else if (binOp::opcode(memory[pc]) == 0x00000022)
			{ // SUB -- Subtract
				setRegister(binOp::registerD(memory[pc]), getRegister(binOp::registerS(memory[pc])) - getRegister(binOp::registerT(memory[pc])));
			}
			else if (binOp::opcode(memory[pc]) == 0x00000018)
			{ // MULT -- Multiply
				low  = getRegister(binOp::registerS(memory[pc])) * getRegister(binOp::registerT(memory[pc]));
				high = low;
			}
			else if (binOp::opcode(memory[pc]) == 0x00000019)
			{ // MULTU -- Multiply Unsigned
				low  = ((unsigned) getRegister(binOp::registerS(memory[pc]))) * ((unsigned) getRegister(binOp::registerT(memory[pc])));
				high = low;
			}
			else if (binOp::opcode(memory[pc]) == 0x0000001a)
			{ // DIV -- Divide
				low  = getRegister(binOp::registerS(memory[pc])) / getRegister(binOp::registerT(memory[pc]));
				high = getRegister(binOp::registerS(memory[pc])) % getRegister(binOp::registerT(memory[pc]));
			}
			else if (binOp::opcode(memory[pc]) == 0x0000001b)
			{ // DIVU -- Divide unsigned
				low  = ((unsigned int) getRegister(binOp::registerS(memory[pc]))) / ((unsigned int) getRegister(binOp::registerT(memory[pc])));
				high = ((unsigned int) getRegister(binOp::registerS(memory[pc]))) % ((unsigned int) getRegister(binOp::registerT(memory[pc])));
			}
			else if (binOp::opcode(memory[pc]) == 0x00000010)
			{ // MFHI -- Move from high/remainder
				setRegister(binOp::registerD(memory[pc]), high);
			}
			else if (binOp::opcode(memory[pc]) == 0x00000012)
			{ // MFLO -- Move from low/quotient
				setRegister(binOp::registerD(memory[pc]), low);
			}
			else if (binOp::opcode(memory[pc]) == 0x00000014)
			{ // LIS -- Load immediate and skip
				setRegister(binOp::registerD(memory[pc]), memory[pc + 1]);
				pc++;
			}
			else if (binOp::opcode(memory[pc]) == 0x0000002a)
			{ // SLT -- Set less than
				setRegister(binOp::registerD(memory[pc]), (getRegister(binOp::registerS(memory[pc])) < getRegister(binOp::registerT(memory[pc]))) ? 1 : 0);
			}
			else if (binOp::opcode(memory[pc]) == 0x0000002b)
			{ // SLTU -- Set less than unsigned
				setRegister(binOp::registerD(memory[pc]), (getRegisterUnsigned(binOp::registerS(memory[pc])) < getRegisterUnsigned(binOp::registerT(memory[pc]))) ? 1 : 0);
			}
			else if (binOp::opcode(memory[pc]) == 0x00000008)
			{ // JR -- Jump register
				pc = getRegister(binOp::registerS(memory[pc]));
				if (pc == returnAddress) break;				
			}
			else if (binOp::opcode(memory[pc]) == 0x00000009)
			{ // JALR -- Jump and link register
				int temp = getRegister(binOp::registerS(memory[pc]));
				registers[31] = pc;
				pc = temp;
				if (pc == returnAddress) break;
			}
		}
		else if (binOp::firstSixBits(memory[pc]) == 0x8C000000)
		{
			/* LW */
			if (binOp::lastFourBytes(memory[pc]) / 4)
			{
				std::cerr << "Unaligned memory access" << std::endl;
				break;
			}

			if ((getRegister(binOp::registerS(memory[pc])) + binOp::lastFourBytes(memory[pc]) / 4) == inputAddress)
			{
				std::string s;
				std::cin >> s;
				registers[binOp::registerT(memory[pc])] = StringUtils::stoi(s);
			}
			else
			{

				int offset;
				if (binOp::lastFourBytes(memory[pc]) > 32768)
					offset += binOp::lastFourBytes(memory[pc]) - 65536;
				else
					offset += binOp::lastFourBytes(memory[pc]);

				setRegister(binOp::registerT(memory[pc]), memory[getRegister(binOp::registerS(memory[pc])) + offset / 4]);
			}
			
		}
		else if (binOp::firstSixBits(memory[pc]) == 0xAC000000)
		{
			/* SW */
			if (binOp::lastFourBytes(memory[pc]) / 4)
			{
				std::cerr << "Unaligned memory access" << std::endl;
				break;
			}

			if ((getRegister(binOp::registerS(memory[pc])) + binOp::lastFourBytes(memory[pc]) / 4) == outputAddress) {
				std::cout << (char) getRegister(binOp::registerT(memory[pc]));
			}
			else
			{

				int offset;
				if (binOp::lastFourBytes(memory[pc]) > 32768)
					offset += binOp::lastFourBytes(memory[pc]) - 65536;
				else
					offset += binOp::lastFourBytes(memory[pc]);

				memory[getRegister(binOp::registerS(memory[pc])) + offset / 4] = getRegister(binOp::registerT(memory[pc]));
			}
			
		}
		else if (binOp::firstSixBits(memory[pc]) == 0x10000000)
		{
			/* BEQ */
			if (getRegister(binOp::registerS(memory[pc])) == getRegister(binOp::registerT(memory[pc]))) {
				if (binOp::lastFourBytes(memory[pc]) > 32768)
					pc += binOp::lastFourBytes(memory[pc]) - 65536;
				else 
					pc += binOp::lastFourBytes(memory[pc]);
			}
		}
		else if (binOp::firstSixBits(memory[pc]) == 0x14000000)
		{
			/* BNE */
			if (getRegister(binOp::registerS(memory[pc])) != getRegister(binOp::registerT(memory[pc]))) {
				if (binOp::lastFourBytes(memory[pc]) > 32768)
					pc += binOp::lastFourBytes(memory[pc]) - 65536;
				else 
					pc += binOp::lastFourBytes(memory[pc]);
			}
		}
		else
		{
			std::cerr << "Invalid opcode" << std::endl;
			break;
		}

		pc++;
	}
}