#include "fast-random-factory.h"
#include "program-hash.h"

#include <iostream>

int main()
{
	CStdRandom rand;
	FastRandomFactory fact(&rand, 20);
	
	/*
	for(auto i = 0; i < 200; ++i)
	{
		auto program = fact.createNewProgram().program;
		for(auto j = 0; j < program.size(); ++j)
		{
			std::cout << "/" << (unsigned)program[j];
		}
		std::cout << std::endl;
	}
	*/
	
	std::unordered_set<Program> programs;
	for(auto i = 1; i <= 0x1000000; ++i)
	{
		auto program = fact.createNewProgram();
		programs.insert(program);
		if(!(i & (i - 1))) std::cout << programs.size() << "," << i << "," << 100 * programs.size() / i << std::endl;
	}
	
	return 0;
}