#include "instruction-set.h"
#include "math-stack.h"
#include "ivm.h"

typedef enum
{
	None = 0,
	DumpStackOnEntry = 1,
	DumpStackOnExit = 2,
	DumpStackAfterEachInstruction = 4
}
DebugMode;

class VM : public IVM
{
	private:
		MathStack stack;
		
		float* memory;
		int memorySize;
		
		// array of InstructionCount function pointers all set to null
		// function pointers point to instruction methods on MathStack
		Result (MathStack::*handlers[InstructionCount])() = { nullptr };
		
		std::vector<int> program;
		
		// debug options
		DebugMode debugMode;
		
		Result result;
		
	public:
		VM(int stackSize, int memorySize);
		
		void setDebugMode(DebugMode flags);
		
		int getInstructionCount();
		
		void loadProgram(const std::vector<int>& program);
		
		bool run(float seed, float* output, int maxOps);
		
		std::string getLastError();
		
		~VM();
};