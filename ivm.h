#pragma once

#include "types.h"

#include <vector>
#include <string>

// virtual machine interface
// virtual machines can run programs and produce output
class IVM
{
	public:
		// returns the name of this vm
		virtual std::string getName() = 0;
		
        virtual int supportedInstructionCount() = 0;
		
		// load a program
		virtual void loadProgram(const Program& program) = 0;

		virtual std::string returnStringForm(const Program& program) = 0;

        // returns false if the program failed to produce output
		virtual bool run(float seed, float* output) = 0;

        // why the program failed to produce output
		virtual std::string getLastError() = 0;
};
