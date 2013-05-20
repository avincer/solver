#include <iostream>

#include "vm.h"

int main()
{
	std::vector<int> noOutput { drop };
	
	std::vector<int> fib
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
	
	std::vector<int> lin
	{
		mul, push1, add
	};
	
	int stackSize = 16, memorySize = 16, maxOps = 100;
	auto debugMode = None; // (DebugMode)(DumpStackOnEntry | DumpStackAfterEachInstruction | DumpStackOnExit);
	
	VM vm(stackSize, memorySize);
	vm.loadProgram(lin);
	vm.setDebugMode(debugMode);
	
	int i = 0;
	float output;

	do
	{
		bool result = vm.run(i, &output, maxOps);
		if(debugMode) std::cout << std::endl;
		
		if(result)
		{
			std::cout << i << ":\t" << output << std::endl;
		}
		else
		{
			std::cout << i << ":\t" << vm.getLastError() << std::endl;
		}
		
		if(debugMode) std::cout << std::endl << std::endl;
		
		++i;
	}
	while(i <= 10);
	
	return 0;
}