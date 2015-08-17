#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <cstdlib> 
#include <sstream>
#include <iomanip>

#include "StringUtils.h"
#include "Machine.h"

int main (int argc, char *argv[]) {

	// Command line options
	bool trace          = false; // -t
	bool printRegisters = false; // -r
	bool print3         = false; // -3

	int filenameIndex   = 1;

	for (int i = 1; ; i++) {

		if ((std::string) argv[i] == "-t") {
			trace = true;
			filenameIndex++;
			continue;
		} else if ((std::string) argv[i] == "-r") {
			printRegisters = true;
			filenameIndex++;
			continue;
		} else if ((std::string) argv[i] == "-3") {
			print3 = true;
			filenameIndex++;
			continue;
		}

		break;
	}

	// Read from file
	std::ifstream input ((std::string) argv[filenameIndex]);

	// Prepare machine
	Machine machine = Machine(65536);
	
	machine.setRegister(1, StringUtils::stoi(argv[filenameIndex + 1]));
	machine.setRegister(2, StringUtils::stoi(argv[filenameIndex + 2]));

	machine.loadProgram(input);
	machine.execute(trace);

	if (print3) {
		std::cout << "Register $3: " << machine.getRegister(3) << std::endl;
	}

	if (printRegisters) {
		for (int i = 0; i < 32; i++) {
			if (machine.getRegister(i) != 0) {
				std::cout << "Register "  ;

				if (i < 10)
					std::cout << " ";

				std::cout << "$" << i << ": " <<std::left << std::setw(10) << machine.getRegister(i)  << std::right<< "0x" << std::hex << machine.getRegister(i) << std::dec << std::endl;
			}
		}
	}

	return machine.getRegister(3);
}