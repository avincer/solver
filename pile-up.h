#include "math-stack.h"
#include "ivm.h"

namespace PileUp
{

// debug modes
typedef enum
{
	None = 0,
	DumpStackOnEntry = 1,
	DumpStackOnExit = 2,
	DumpStackAfterEachInstruction = 4
}
DebugMode;

// the instruction set
typedef enum
{
	// push //
	// note: these must be contiguous and in order!
	pushNeg1, // = 0
	push0,
	push1,
	push2,
	push3,
	push4,
	push5,
	push6,
	push7,
	push8,
	push9,
	push10,
	
	// stack //
	dup, // = 12
	swap,
	drop,
	
	// unary math //
	neg, // = 15
	inc,
	dec,
	truncate,
	
	// unary logic //
	lnot, // = 19
	
	// binary math //
	add, // = 20
	sub,
	mul,
	divide,
	fmodulus,
	
	// binary logic //
	land, // = 25
	lor,
	
	// comparators //
	eq, // = 27
	neq,
	gt,
	lt,
	gte,
	lte,
	
	// flow control
	jif, // = 33
	jmp,
	ret,
	
	// memory
	get, // = 36
	put,
	
	InstructionCount // must be last!
}
Instruction;

class VM : public IVM
{
	private:
		MathStack stack;
		
		float* memory;
		int stackSize, memorySize, maxOps;
		
		// vector of function pointers called handlers
		// each entry is a method on MathStack
		std::vector<Result (MathStack::*)()> handlers;
		
		Program program;
		
		// debug options
		DebugMode debugMode;
		
		Result result;
		
	public:
		VM(int stackSize, int memorySize, int maxOps);
		
		void setDebugMode(DebugMode flags);
		
		std::string getName();
		
        int supportedInstructionCount();
		
		void loadProgram(const Program& program);
		
		std::string returnStringForm(const Program& program);
		
		bool run(float seed, float* output);
		
		std::string getLastError();
		
		~VM();
	
	private:
		const char* translateInstruction(Instruction instruction);
};

}
