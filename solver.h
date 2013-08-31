#include "program-factory.h"
#include "ivm.h"
#include "timer.h"

#include <list>

// solver ties together components to generate, run and score programs
class Solver
{
	private:
		// how many of the best programs to display
		static const int bestProgramCount = 10;
		
		// how many programs to run between updates
		static const int updatePeriod = 1000000;
		
		// the best programs so far, ordered by score (best first)
		std::list<ProgramInfo> bestPrograms;
		
		// factory and vm to be used
		IProgramFactory* factory;
		IVM* vm;
		
		// the target sequence and output sequence for the current program
		std::vector<float> target, output;

		// status
		size_t programCount;
		bool running;
		
		// use a timer to measure run time
		Timer timer;
		
		// the scoring function
		// computes score from target and output vectors
		double scoreOutput(int outputLen);
		
		// updates the list of best programs with program
		void updateBestPrograms(ProgramInfo& program, int outputLen);
		
		void outputStatus();
		
		void outputSequence(const std::vector<float>& sequence);
	
	public:
		// app version, defined in solver.cpp
		static const char* version;
		
		Solver(IProgramFactory* factory, IVM* vm, 
			   const std::vector<float>& target);

		void run(size_t maxPrograms, bool exitOnFirstSolution, double brevityWeight);
		
		void stop();
		
		// todo - load function
		
		void save(const std::string& fileName);
		
		void dumpProgramInformation(const Program& program);
};