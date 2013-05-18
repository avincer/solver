#include "instruction-set.h"
#include "math-stack.h"

typedef enum
{
	None = 0,
	DumpStackOnEntry = 1,
	DumpStackOnExit = 2,
	DumpStackAfterEachInstruction = 4
}
DebugMode;

class VM
{
	private:
		MathStack stack;
		
		float* memory;
		int memorySize;
		
		// array of InstructionCount function pointers all set to null
		// function pointers point to instruction methods on MathStack
		Result (MathStack::*handlers[InstructionCount])() = { nullptr };
		
		Instruction* program;
		int programLen;
		
		// debug options
		DebugMode debugMode;
		
	public:
		VM(int stackSize, int memorySize);
		
		void setDebugMode(DebugMode flags);
		
		void loadProgram(Instruction* program, int programLen);
		
		Result run(float seed, float* output, int maxOps);
		
		~VM();
};