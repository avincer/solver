#include "tree.h"
#include "pile-up.h"
#include "timer.h"

#include <iostream>

using namespace PileUp;

int main()
{
	CStdRandom random;
	random.init(time(nullptr));
	
	std::vector<double> initialWeights(InstructionCount, 0.5);
	ProgramTree tree(&random, initialWeights);
	
	Timer timer;
	timer.start();
	
	for(int b = 0; b < 10; ++b)
	{
		auto start = clock();
		int count = 100000;
		
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
		
		auto elapsed = timer.getElapsedTime(true);
		std::cout << "Added " << count << " new nodes in " << elapsed << " seconds (" << count / elapsed << " nodes/second" << std::endl;
	}
}