#include "instruction-set.h"

const char* translateInstruction(Instruction instruction)
{
	switch(instruction)
	{
		case pushNeg1:
			return "pushNeg1";
		case push0:
			return "push0";
		case push1:
			return "push1";
		case push2:
			return "push2";
		case push3:
			return "push3";
		case push4:
			return "push4";
		case push5:
			return "push5";
		case push6:
			return "push6";
		case push7:
			return "push7";
		case push8:
			return "push8";
		case push9:
			return "push9";
		case push10:
			return "push10";
			
		case dup:
			return "dup";
		case swap:
			return "swap";
		case drop:
			return "drop";
			
		case neg:
			return "neg";
		case inc:
			return "inc";
		case dec:
			return "dec";
		case truncate:
			return "truncate";
			
		case lnot:
			return "lnot";
			
		case add:
			return "add";
		case sub:
			return "sub";
		case mul:
			return "mul";
		case divide:
			return "divide";
		case fmodulus:
			return "fmodulus";
			
		case land:
			return "land";
		case lor:
			return "lor";
			
		case eq:
			return "eq";
		case neq:
			return "neq";
		case gt:
			return "gt";
		case lt:
			return "lt";
		case gte:
			return "gte";
		case lte:
			return "lte";
			
		case jif:
			return "jif";
		case jmp:
			return "jmp";
		case ret:
			return "ret";
			
		case get:
			return "get";
		case put:
			return "put";
			
		case InstructionCount:
			throw "Tried to translate InstructionCount!";
		default:
			return "???";
	}
}