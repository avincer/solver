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
		static const int updatePeriod = 100000;
		
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
		
		// use a timer to measure run time
		Timer timer;
		
		// remember time of last update for computing speed
		double lastUpdateTime;
		
		// the scoring function
		// computes score from target and output vectors
		double computeScore(int outputLen);
		
		// updates the list of best programs with program
		void updateBestPrograms(Program& program, int outputLen);
		
		void outputStatus();
		
		void outputSequence(const std::vector<float>& sequence);
	
	public:
		// app version, defined in solver.cpp
		static const char* version;
		
		Solver(IProgramFactory* factory, IVM* vm, 
			   const std::vector<float>& target);

		void run();
		
		void stop();
		
		// todo - load function
		
		void save(const std::string& fileName);
		
		void dumpProgramInformation(const std::vector<int>& program);
};