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
	pushNeg1,
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
	dup,
	swap,
	drop,
	
	// unary math //
	neg,
	inc,
	dec,
	truncate,
	
	// unary logic //
	lnot,
	
	// binary math //
	add,
	sub,
	mul,
	divide,
	fmodulus,
	
	// binary logic //
	land,
	lor,
	
	// comparators //
	eq,
	neq,
	gt,
	lt,
	gte,
	lte,
	
	// flow control
	jif,
	jmp,
	ret,
	
	// memory
	get,
	put,
	
	InstructionCount // must be last!
}
Instruction;

class VM : public IVM
{
	private:
		MathStack stack;
		
		float* memory;
		int stackSize, memorySize;
		
		// vector of function pointers called handlers
		// each entry is a method on MathStack
		std::vector<Result (MathStack::*)()> handlers;
		
		std::vector<int> program;
		
		// debug options
		DebugMode debugMode;
		
		Result result;
		
	public:
		VM(int stackSize, int memorySize);
		
		void setDebugMode(DebugMode flags);
		
		std::string getName();
		
		int getInstructionCount();
		
		void loadProgram(const std::vector<int>& program);
		
		bool run(float seed, float* output, int maxOps);
		
		std::string getLastError();
		
		~VM();
	
	private:
		const char* translateInstruction(Instruction instruction);
};

}