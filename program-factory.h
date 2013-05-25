#pragma once

#include <vector>
#include <string>

// a program
typedef struct
{
	// program instructions
	std::vector<int> instructions;
	
	// score for this program
	double score;
	
	// output for this program, if recorded
	std::vector<float> output;
	
	// node that represents this program (used by program factory)
	void* node;
}
Program;

// program factory interface
// program factories are responsible for intelligently creating new 
// programs based upon the scores of previously run programs
class IProgramFactory
{
	public:
		// returns the name of this factory
		virtual std::string getName() = 0;
		
		// returns a new program
		virtual Program createNewProgram() = 0;
		
		// updates internal statistics with the score for this program
		// note: requires program.score to be set!
		virtual void recordProgramScore(Program program) = 0;
		
		// todo - add functions to load and save state
};