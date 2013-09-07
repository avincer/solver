#include <iostream>

#include "pile-up.h"
#include "fenv.h"

using namespace PileUp;

int main()
{
	feenableexcept(FE_DIVBYZERO | FE_INVALID);
	
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
		push3, mul, push1, add
	};
	
	std::vector<int> alt
	{
		dup, push2, fmodulus, push2, mul, dec, mul
	};
	
	std::vector<int> quad1
	{
		dup, inc, mul, push3, sub
	};
	
	Program fmodOfInf
	{
	  4, 22, 12, 17, 4, 24, 21, 16, 11, 15, 34
	};
	
	int stackSize = 16, memorySize = 16, maxOps = 1000;
	auto debugMode = (DebugMode)(DumpStackOnEntry | DumpStackAfterEachInstruction | DumpStackOnExit);
	
	VM vm(stackSize, memorySize, maxOps);
	vm.loadProgram(fmodOfInf);
	vm.setDebugMode(debugMode);
	
	int i = 0;
	float output;

	do
	{
		bool result = vm.run(i, &output);
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