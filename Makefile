CXX = g++
CXXFLAGS = -O0 -fmax-errors=2 -std=c++11
EXEC = MIPS-Emulator
OBJECTS = MIPS-Emulator.o StringUtils.o BinaryOperations.o Machine.o Memory.o 
DEPENDS = ${OBJECTS:.o=.d}

-include ${DEPENDS}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

.PHONY: clean

clean:
	-rm ${OBJECTS} ${EXEC}
