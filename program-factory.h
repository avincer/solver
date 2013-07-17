#pragma once

#include "types.h"

#include <vector>
#include <string>
#include <ostream>

typedef struct
{
	// the program
	Program program;
	
	// score for this program
	double score;
	
	// output for this program, if recorded
	std::vector<float> output;
	
	// node that represents this program (used by program factory)
	void* node;
}
ProgramInfo;

// program factory interface
// program factories are responsible for intelligently creating new 
// programs based upon the scores of previously run programs
class IProgramFactory
{
	public:
		// returns the name of this factory
		virtual std::string getName() = 0;
		
		// returns a new program
		virtual ProgramInfo createNewProgram() = 0;
		
		// updates internal statistics with the score for this program
		// note: requires program.score to be set!
		virtual void recordProgramScore(ProgramInfo program) = 0;
		
		virtual void toXml(std::ostream& stream) = 0;
		
		// output information about a program
		virtual void dumpProgramInformation(const Program& program) = 0;
};