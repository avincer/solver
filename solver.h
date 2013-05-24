#include "program-factory.h"
#include "ivm.h"

#include <vector>

class Solver
{
	private:
		std::vector<float> target, output;
		bool running;
		
		double computeScore(int targetLen)
		{
			// something...
			return -1;
		}
	
	public:
		void run(IProgramFactory* factory, IVM* vm, 
				 const std::vector<float>& targetSequence, int maxOps)
		{
			this->target = targetSequence;
			output.resize(target.size());
			running = true;
			
			while(running)
			{
				Program program = factory->createNewProgram();
				vm->loadProgram(program.instructions);
				
				bool result = true;
				int i = 0;
				for(; (i < target.size()) && result; ++i)
				{
					result = vm->run(i, &output[i], maxOps);
				}
				
				double score = computeScore(i);
				factory->setProgramScore(program, score);
				
				// todo - store program and output in hall of fame if it did well
				// todo - periodically output the best programs
			}
		}
		
		void stop()
		{
			running = false;
		}
		
		// something to do with loading / saving here?
};