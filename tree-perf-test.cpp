#include "tree.h"
#include <iostream>
#include <ctime>

int main()
{
	CStdRandom random;
	random.init(time(nullptr));
	
	double initialWeights[instructionCount];
	for(int i = 0; i < instructionCount; ++i)
	{
		initialWeights[i] = 0.5;
	}
	
	ProgramTree tree(&random, initialWeights);
	for(int b = 0; b < 100; ++b)
	{
		auto start = clock();
		int count = 10000;
		
		for(int i = 0; i < count; ++i)
		{
			Program p = tree.createNewProgram();
			/*
			std::cout << "New program: ";
			for(int j = 0; j < p.instructionCount; ++j)
			{
				std::cout << p.instructions[j] << " ";
			}
			std::cout << std::endl;
			*/
			
			// todo: run program here!
			double score = random.getDouble(1);
			tree.setProgramScore(p, score);
		}
		
		auto elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
		//std::cout << "Added " << count << " new nodes in " << elapsed << " seconds" << std::endl;
	}
}