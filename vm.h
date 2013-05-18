#include "instruction-set.h"
#include "math-stack.h"

class VM
{
	private:
		MathStack stack;
		
		float* memory;
		int memorySize;
		
		// array of InstructionCount function pointers all set to null
		Result (MathStack::*handlers[InstructionCount])() = { nullptr };
		
		Instruction* program;
		int programLen;
		int maxOps;
		
	public:
		VM(int stackSize, int memorySize);
		
		void loadProgram(Instruction* program, int programLen, int maxOps);
		
		// todo - return reason for failure
		Result run(float seed, float* output);
		
		~VM();
};