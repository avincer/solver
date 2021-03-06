#include "random-factory.h"

#include <cmath>

RandomFactory::RandomFactory(IRandom* random, unsigned char instructionCount)
	: random(random), instructionCount(instructionCount)
{
	// pre-compute the number of permutations for different lengths of program
	// up to the length we can store (which isn't very long)
	statCount = floor(sizeof(size_t) * 8 * log(2) / log(instructionCount));
	stats.resize(statCount + 1);
	
	size_t count = 1;
	for(auto i = 1; i <= statCount; ++i)
	{
		count *= instructionCount;
		stats[i].count = stats[i].remaining = count;
	}
	
	// note: by default unordered set has 11 buckets
	// reserve lots of space so it doesn't re-hash like mad once we start
	programs.reserve(10000000);
}

std::string RandomFactory::getName()
{
	return "RandomFactory";
}

bool RandomFactory::addProgram(const Program& program)
{
	// note: guessing is fine when creating random programs, 
	// but we must be precise here!
	auto len = program.size();
	if(len <= statCount && !stats[len].remaining) return false;
	auto added = programs.insert(program).second;
	if(added && len <= statCount) --stats[len].remaining;
	return added;
}

// returns a new program
ProgramInfo RandomFactory::createNewProgram()
{
	programInfo.program.clear();
	size_t len = 0;
	auto createdProgram = false;
	
	do
	{
		programInfo.program.push_back(random->getInt(instructionCount));
		++len;
		
		auto tryCreateProgram = true;
		
		if(len <= statCount)
		{
			// if there are no programs remaining of this length we cannot create one
			if(!stats[len].remaining)
			{
				tryCreateProgram = false;
			}
			else
			{
				// if there are 10% of programs of this length remaining, check 10% of the time
				// assume program already exists 90% of the time (even though it may not)
				// note: favours generation of slightly longer programs
				auto chance = 1.0 - stats[len].remaining / (double)stats[len].count;
				if(random->maybe(chance))
				{
					tryCreateProgram = false;
				}
			}
		}
		
		if(tryCreateProgram)
		{
			// note: second is true if program was created, false if it already exists
			createdProgram = programs.insert(programInfo.program).second;
			if(createdProgram && len <= statCount) --stats[len].remaining;
		}
	}
	while(!createdProgram);
	
	return programInfo;
}