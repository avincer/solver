#pragma once

#include <vector>
#include <string>

// virtual machine interface
// virtual machines can run programs and produce output
class IVM
{
	public:
		// returns the name of this vm
		virtual std::string getName() = 0;
		
		// get how many instructions this VM supports
		virtual int getInstructionCount() = 0;
		
		// load a program
		virtual void loadProgram(const std::vector<int>& program) = 0;
		
		// format the program as a string
		virtual std::string formatProgram(const std::vector<int>& program) = 0;
		
		// run the program
		// returns false if the program failed to produce output for any reason
		virtual bool run(float seed, float* output, int maxOps) = 0;
		
		// returns information about why the program failed to produce output
		virtual std::string getLastError() = 0;
};