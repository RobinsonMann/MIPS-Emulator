#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <cstdlib> 
#include <sstream>

#include "StringUtils.h"
#include "Machine.h"

int main (int argc, char *argv[]) {

	std::ifstream input ((std::string) argv[1]);

	Machine machine = Machine(65536);
	
	machine.setRegister(1, StringUtils::stoi(argv[2]));
	machine.setRegister(2, StringUtils::stoi(argv[3]));

	machine.loadProgram(input);
	machine.execute();

	std::cout << "Register $3: " << machine.getRegister(3) << std::endl;

	return 0;
}