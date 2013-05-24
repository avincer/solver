#pragma once

#include <vector>

typedef struct
{
	std::vector<int> instructions;
	void* node;
}
Program;

class IProgramFactory
{
	public:
		virtual Program createNewProgram() = 0;
		
		virtual void setProgramScore(Program program, double score) = 0;
		
		// todo - add functions to load and save state
};