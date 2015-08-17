#MIPS Emulator (University of Waterloo CS241 Dialect)

This is the source code of an emulator for the CS241 Dialect of the MIPS instruction set architecture.

A copy of the instruction set can be found here: https://www.student.cs.uwaterloo.ca/~cs241/mips/mipsref.pdf

## Usage

`./MIPS-Emulator.exe <Mips Machine Code> <Register 1> <Register 2>`

## IO

As outlined in instruction set (https://www.student.cs.uwaterloo.ca/~cs241/mips/mipsref.pdf)

> When a word is stored to memory location 0xFFFF000C the least-signifcant byte (eight bits) of the word are sent to the standard output.
>
> Loading a word from memory location 0xFFFF0004 places the next byte from standard input into the least-signifcant byte of the destination register.

## Files

**MIPS-Emulator** -- Main file
**Machine**	-- MIPS machine, can load and execute programs
**Memory** -- RAM
**StringUtils** -- String utilities
**BinaryOperations** -- Binary parsing operations on 32 bit ints



## License

The MIT License (MIT)

Copyright (c) 2015, Robinson Mann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


