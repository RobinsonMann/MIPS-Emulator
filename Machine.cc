#include "Machine.h"

#include <iomanip>

/* Constructor */
Machine::Machine (int size) : memory(Memory(size)), pc(0) {
	registers[31] = returnAddress; // Return addresss
	registers[30] = size * 4;      // Last address in memory
	for (int i = 0; i < 30; i++) registers[i] = 0;
}

/* Load a MIPS program into the machine*/
void Machine::loadProgram(std::istream &in) { memory.loadProgram(in); }

void Machine::setRegister(int reg, int value) {
	if      (reg == 0) return;
	else if (reg <  1) std::cerr << "Tried to set register < 1"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to set register > 31" << std::endl;
	else               registers[reg] = value;
}

int Machine::getRegister(int reg) {
	if      (reg <  0) std::cerr << "Tried to access register < 0"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to access register > 31" << std::endl;
	else               return registers[reg];
}

	void Machine::setRegisterUnsigned(int reg, unsigned int value) {

	if      (reg == 0) return;
	else if (reg <  1) std::cerr << "Tried to set register < 1"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to set register > 31" << std::endl;
	else               registers[reg] = value;
}

unsigned int Machine::getRegisterUnsigned(int reg) {
	if      (reg <  0) std::cerr << "Tried to access register < 0"  << std::endl;
	else if (reg > 31) std::cerr << "Tried to access register > 31" << std::endl;
	else               return registers[reg];
}
 
// Anon namespace forces these functions to only be accessable to this file
namespace {
	enum instructionType {
		add,
		sub, 
		mult,
		multu,
		divIns,
		divu,
		mfhi,
		mflo,
		lis,
		lw,
		sw,
		slt,
		sltu,
		beq,
		bne, 
		jr,
		jalr,
		unknown,
	};

	instructionType typeofInstruction(int instruction);
	std::string instructionAsString(instructionType type);
}

/* Starts execution of the machine */
void Machine::execute(bool trace) {

	while (true)
	{
		int instruction = memory[pc / 4];
		
		int firstSixBits = binOp::firstSixBits(instruction);
		int lastFourBytes = binOp::lastFourBytes(instruction);

		int opcode = binOp::opcode(instruction);
		
		int registerS = binOp::registerS(instruction);
		int registerT = binOp::registerT(instruction);
		int registerD = binOp::registerD(instruction);

		pc += 4;
		instructionType instructionType = typeofInstruction(instruction);

		if (trace) std::cerr << std::left << std::setw(6) << pc << instructionAsString(instructionType) << std::endl;

		if (instructionType == add)
		{
			setRegister(registerD, getRegister(registerS) + getRegister(registerT));
		}
		else if (instructionType == sub)
		{ 
			setRegister(registerD, getRegister(registerS) - getRegister(registerT));
		}
		else if (instructionType == mult)
		{ 
			low  = getRegister(registerS) * getRegister(registerT);
			high = low;
		}
		else if (instructionType == multu)
		{ 
			low  = ((unsigned) getRegister(registerS)) * ((unsigned) getRegister(registerT));
			high = low;
		}
		else if (instructionType == divIns)
		{ 
			low  = getRegister(registerS) / getRegister(registerT);
			high = getRegister(registerS) % getRegister(registerT);
		}
		else if (instructionType == divu)
		{
			low  = ((unsigned int) getRegister(registerS)) / ((unsigned int) getRegister(registerT));
			high = ((unsigned int) getRegister(registerS)) % ((unsigned int) getRegister(registerT));
		}
		else if (instructionType == mfhi)
		{ 
			setRegister(registerD, high);
		}
		else if (instructionType == mflo)
		{
			setRegister(registerD, low);
		}
		else if (instructionType == lis)
		{
			setRegister(registerD, memory[pc / 4]);
			pc += 4;
		}
		else if (instructionType == slt)
		{
			setRegister(registerD, (getRegister(registerS) < getRegister(registerT)) ? 1 : 0);
		}
		else if (instructionType == sltu)
		{
			setRegister(registerD, (getRegisterUnsigned(registerS) < getRegisterUnsigned(registerT)) ? 1 : 0);
		}
		else if (instructionType == jr)
		{	
			pc = getRegister(registerS); 
			if (pc == returnAddress) break;
			pc = pc;
			continue;
		}
		else if (instructionType == jalr)
		{	
			int temp = getRegister(registerS);
			registers[31] = pc;
			pc = temp;
			if (temp == returnAddress) break;
			continue;
		}
		else if (instructionType == lw)
		{

			if (lastFourBytes % 4 != 0)
			{
				std::cerr << "Unaligned memory access" << std::endl;
				break;
			}

			if ((getRegister(registerS) + lastFourBytes / 4) == inputAddress)
			{
				std::string s;
				std::cin >> s;
				registers[registerT] = StringUtils::stoi(s);
			}
			else
			{

				int offset;
				if (lastFourBytes > 32768)
					offset = lastFourBytes - 65536;
				else
					offset = lastFourBytes;

				setRegister(registerT, memory[getRegister(registerS) / 4 + offset / 4]);
			}
			
		}
		else if (instructionType == sw)
		{
			/* SW */
			if (lastFourBytes % 4 != 0)
			{
				std::cerr << "Unaligned memory access" << std::endl;
				break;
			}

			if ((getRegister(registerS) + lastFourBytes / 4) == outputAddress) {
				std::cout << (char) getRegister(registerT);
			}
			else
			{

				int offset;
				if (lastFourBytes > 32768)
					offset = lastFourBytes - 65536;
				else
					offset = lastFourBytes;

				memory[getRegister(registerS) / 4 + offset / 4] = getRegister(registerT);
			}
			
		}
		else if (instructionType == beq)
		{

			/* BEQ */
			if (getRegister(registerS) == getRegister(registerT)) {

				if (lastFourBytes > 32768) {
					pc += (lastFourBytes - 65536) * 4;
				} else {
					pc += (lastFourBytes) * 4;
				}
			}
		}
		else if (instructionType == bne)
		{
			/* BNE */
			if (getRegister(registerS) != getRegister(registerT))
			{
				if (lastFourBytes > 32768) {
					pc += (lastFourBytes - 65536) * 4;
				} else {
					pc += (lastFourBytes) * 4;
				}
			}
		}
		else
		{
			std::cerr << "Invalid opcode" << std::endl;
			break;
		}


	}
}

namespace {
		instructionType typeofInstruction(int instruction) {

			int firstSixBits = binOp::firstSixBits(instruction);
			int opcode = binOp::opcode(instruction);
		
			if (firstSixBits == 0x00000000) {
				if (opcode == 0x00000020) return add;
				else if (opcode == 0x00000022) return sub;
				else if (opcode == 0x00000018) return mult;
				else if (opcode == 0x00000019) return multu;
				else if (opcode == 0x0000001a) return divIns;
				else if (opcode == 0x0000001b) return divu;
				else if (opcode == 0x00000010) return mfhi;
				else if (opcode == 0x00000012) return mflo;
				else if (opcode == 0x00000014) return lis;
				else if (opcode == 0x0000002a) return slt;
				else if (opcode == 0x0000002b) return sltu;
				else if (opcode == 0x00000008) return jr;
				else if (opcode == 0x00000009) return jalr;
				else return unknown;
			}
			else if (firstSixBits == 0x8C000000)
				return lw;	
			else if (firstSixBits == 0xAC000000)
				return sw;			
			else if (firstSixBits == 0x10000000)
				return beq;
			else if (firstSixBits == 0x14000000)
				return bne;
			else
				return unknown;
	}

	std::string instructionAsString(instructionType type) {
		// C++98 doesn't support default hash maps, so linear is good enough.
		if (type == add) return "add";
		if (type == sub) return "sub"; 
		if (type == mult) return "mult";
		if (type == multu) return "multu";
		if (type == divIns) return "divIns";
		if (type == divu) return "divu";
		if (type == mfhi) return "mfhi";
		if (type == mflo) return "mflo";
		if (type == lis) return "lis";
		if (type == lw) return "lw";
		if (type == sw) return "sw";
		if (type == slt) return "slt";
		if (type == sltu) return "sltu";
		if (type == beq) return "beq";
		if (type == bne) return "bne"; 
		if (type == jr) return "jr";
		if (type == jalr) return "jalr";
		return "unknown";
	}
}