#pragma once

#include "types.h"

#include <vector>
#include <string>
#include <ostream>

typedef struct
{
	Program program;
	
    double score; // for this program
	
    std::vector<float> output; // for this program, if recorded
	
    void* node; // represents this program (used by program factory)
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
		virtual void recordProgramScore(const ProgramInfo& program) = 0;
		
		virtual void toXml(std::ostream& stream) = 0;
		
		// output information about a program
		virtual void dumpProgramInformation(const Program& program) = 0;
};
