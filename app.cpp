#include <iostream>

#include "vm.h"

#define len(_x) sizeof(_x) / sizeof(Instruction)

int main()
{
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
	
	int stackSize = 16;
	int memorySize = 16;
	
	VM vm(stackSize, memorySize);
	vm.loadProgram(fib, len(fib), 100);
	
	int i = 0;
	Result result;
	float output;

	do
	{
		std::cout << i << "\t" ;
		result = vm.run(i, &output);
		
		switch(result)
		{
			case Ok:
				std::cout << output << std::endl;
				break;
			case StackUnderflow:
				std::cout << "stack underflow" << std::endl;
				break;
			case StackOverflow:
				std::cout << "stack overflow" << std::endl;
				break;
			case ExceededMaxOpCount:
				std::cout << "exceeded max op count" << std::endl;
				break;
		}
		++i;
	}
	while(i <= 10);
	
	return 0;
}