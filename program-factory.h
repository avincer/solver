#pragma once

#include "types.h"

#include <string>

// program factory interface
// program factories are responsible for intelligently creating new 
// programs based upon the scores of previously run programs
class IProgramFactory
{
	public:
		// returns the name of this factory
		virtual std::string getName() = 0;
		
		// returns a new program
		virtual const Program& createNewProgram() = 0;
		
		// updates internal statistics with the score for this program
		// note: requires program.score to be set!
		virtual void recordProgramScore(const Program& program, double score) = 0;
		
		// output information about a program
		virtual void dumpProgramInformation(const Program& program) = 0;
};