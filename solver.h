#include "program-factory.h"
#include "ivm.h"
#include "timer.h"

#include <vector>
#include <list>
#include <iostream>
#include <iomanip>

// solver ties together components to generate, run and score programs
class Solver
{
	private:
		const char* version = "0.1";
		
		// how many of the best programs to display
		const int bestProgramCount = 10;
		
		// how many programs to run between updates
		const int updatePeriod = 1000;
		
		// the best programs so far, ordered by score (best first)
		std::list<Program> bestPrograms;
		
		// factory and vm to be used
		IProgramFactory* factory;
		IVM* vm;
		
		// the target sequence and output sequence for the current program
		std::vector<float> target, output;

		// status
		int programCount;
		bool running;
		
		// use a timer to measure speed
		Timer timer;
		
		// the scoring function
		// computes score from target and output vectors
		double computeScore(int outputLen)
		{
			// todo - consider whether factoring in the length of the 
			// program is the correct approach
			double score = 0;
			for(int i = 0; i < outputLen; ++i)
			{
				double e = output[i] - target[i];
				score += 1.0 / (1.0 + e * e);
			}
			return score;
		}
		
		// updates the list of best programs with program
		void updateBestPrograms(Program& program, int outputLen)
		{
			if(bestPrograms.size() < bestProgramCount || 
				program.score > bestPrograms.back().score)
			{
				// store the program along with its output
				program.output = output;
				program.output.resize(outputLen);
				
				if(bestPrograms.size() == 0)
				{
					// first program to be stored!
					bestPrograms.push_back(program);
				}
				else
				{
					// insert the program in the correct place
					// todo - maybe use a set for this?
					bool inserted = false;
					for(auto it = bestPrograms.begin(); 
						it != bestPrograms.end() && !inserted; ++it)
					{
						if(program.score > it->score)
						{
							bestPrograms.insert(it, program);
							inserted = true;
						}
					}
					
					if(bestPrograms.size() > bestProgramCount)
					{
						bestPrograms.pop_back();
					}
				}
			}
		}
		
		void outputStatus()
		{
			// solver v0.1
			//
			// factory: ProgramTree
			// vm:		PileUp( vm info )
			// target:  [1, 2, 3, 4, ...]
			// status:  tested xxx programs (current rate yyy programs per second)
			//
			// best programs
			// 1. (score) (program)
			//            (output)
			//            
			// 2. (score) (program)
			//			  (output)
			//
			std::cout << " solver: v" << version << std::endl;
			std::cout << std::endl;
			std::cout << "factory: " << factory->getName() << std::endl;
			std::cout << "     vm: " << vm->getName() << std::endl;
			std::cout << " target: ";
			outputSequence(target);
			std::cout << " status: tested " << programCount << " programs (" 
				<< (int)(updatePeriod / timer.getElapsedTime(true)) << " programs/second)" << std::endl;
			std::cout << std::endl;
			std::cout << " score | output" << std::endl;
			for(auto program: bestPrograms)
			{
				std::cout << std::setw(6) << std::setprecision(3) << program.score;
				std::cout << std::setw(0) << " | ";
				outputSequence(program.output);
			}
			std::cout << std::endl;
		}
		
		void outputSequence(const std::vector<float>& sequence)
		{
			// todo - add support for wrapping
			for(auto x: sequence)
			{
				std::cout << x << " ";
			}
			std::cout << std::endl;
		}
	
	public:
		Solver(IProgramFactory* factory, IVM* vm, 
			   const std::vector<float>& target)
		{
			this->factory = factory;
			this->vm = vm;
			this->target = target;
			output.resize(target.size());
			programCount = 0;
		}

		// todo - why is maxOps here?
		void run(int maxOps)
		{
			running = true;
			timer.start();
			
			while(running)
			{
				// create a program and load it into the VM
				Program program = factory->createNewProgram();
				vm->loadProgram(program.instructions);
				
				// run the program and store output
				bool result = true;
				int i = 0;
				for(; (i < target.size()) && result; ++i)
				{
					result = vm->run(i, &output[i], maxOps);
				}
				
				// score the program and update the program factory stats
				program.score = computeScore(i);
				factory->recordProgramScore(program);
				
				// record it if it was any good
				updateBestPrograms(program, i);

				// output status every so often
				++programCount;
				if(programCount % updatePeriod == 0) outputStatus();
			}
		}
		
		void stop()
		{
			running = false;
		}
		
		// something to do with loading / saving here?
};