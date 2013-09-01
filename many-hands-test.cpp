#include "top-factory.h"
#include "pile-up.h"

std::vector<size_t> bounds { 1000, 10000, 100000, 1000000 };
std::vector<std::unordered_set<Program>> programs;

std::vector<float> target { 2, 3, 5, 7, 11, 13, 17 };
std::vector<float> output;

double scoreOutput(int outputLen)
{
	// todo - consider whether factoring in the length of the 
	// program is the correct approach
	double score = 0;
	for(int i = 0; i < outputLen; ++i)
	{
		double e = output[i] - target[i];
		score += 1.0 / (1.0 + e * e);
	}
	return score / target.size();
}

void doWork(bool first)
{
	output.resize(target.size());
	
	CStdRandom random;
	random.init(time(nullptr));
	
	PileUp::VM vm(16, 16, 1000);
	
	TopFactory factory(&random, vm.supportedInstructionCount(), 100, 0.5);
	
	auto boundPos = 0;
	for(auto b = 1; boundPos < bounds.size(); ++b)
	{
		// create a program and load it into the VM
		auto programInfo = factory.createNewProgram();
		vm.loadProgram(programInfo.program);
		
		// run the program and store output
		bool result = true;
		int i = 0;
		for(; (i < target.size()) && result; ++i)
		{
			result = vm.run(i, &output[i]);
		}
		
		// score the program and update the program factory stats
		auto outputScore = scoreOutput(i);
		auto brevityScore = 1.0 / programInfo.program.size();
		programInfo.score = (1.0 - 0.125) * outputScore + 0.125 * brevityScore;
		factory.recordProgramScore(programInfo);
		
		// compute stats
		if(b == bounds[boundPos])
		{
			if(first)
			{
				programs.push_back(factory.getRandomFactory()->getPrograms());
			}
			else
			{
				for(auto program: factory.getRandomFactory()->getPrograms())
				{
					programs[boundPos].insert(program);
				}
			}
			std::cout << "," << programs[boundPos].size() / (double)b << std::flush;
			boundPos++;
		}
	}
}

int main()
{
	std::cout << "workers/relative work";
	for(auto bound: bounds)
	{
		std::cout << "," << bound;
	}
	std::cout << std::endl;
	
	for(auto i = 1; i <= 8; ++i)
	{
		std::cout << i;
		doWork(i == 1);
		std::cout << std::endl;
	}
	
	return 0;
}