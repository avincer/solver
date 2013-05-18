#include <iostream>

#include "vm.h"

#define len(_x) sizeof(_x) / sizeof(Instruction)

int main()
{
	Instruction noOutput[] = { drop };
	
	Instruction fib[] = 
	{
		// if seed value >= 2 jump forward
		dup, push2, gte, push4, jif,
		
		// store seed value
		dup, push0, put,
		
		push0, get, // fetch previous results
		push1, get,
		dup, push0, put, // shuffle
		add, // work out new result
		dup, push1, put // store it
	};
	
	int stackSize = 16, memorySize = 16, maxOps = 100;
	auto debugMode = (DebugMode)(DumpStackOnEntry | DumpStackAfterEachInstruction | DumpStackOnExit);
	
	VM vm(stackSize, memorySize);
	vm.loadProgram(fib, len(fib));
	//vm.setDebugMode(debugMode);
	
	int i = 0;
	Result result;
	float output;

	do
	{
		result = vm.run(i, &output, maxOps);
		
		switch(result)
		{
			case Ok:
				std::cout << i << ":\t" << output << std::endl;
				break;
			default:
				std::cout << i << ":\t" << translateResult(result) << std::endl;
				break;
		}
		++i;
	}
	while(i <= 10);
	
	return 0;
}