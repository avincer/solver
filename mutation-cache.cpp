#include "mutation-cache.h"

MutationCache::MutationCache(unsigned char instructionCount) :
	instructionCount(instructionCount)
{
}

const std::vector<Mutation>& MutationCache::getMutations(size_t programLen)
{
	if(mutationCache.size() < programLen + 1)
	{
		mutationCache.resize(programLen + 1);
	}
	
	std::vector<Mutation>& mutations = mutationCache[programLen];
	
	if(!mutations.size())
	{
		// generate mutations
		// we have:
		// (programLen + 1) * instructionCount possible additions
		// programLen possible removals
		// programLen * (instructionCount - 1) possible substitutions
		// this adds to instructionCount * (2 * programLen + 1)
		mutations.reserve(instructionCount * (2 * programLen + 1));
		
		for(auto i = 0; i < programLen; ++i)
		{
			mutations.push_back(createMutation(removal, 0, i));
			
			for(auto j = 1; j < instructionCount; ++j)
			{
				mutations.push_back(createMutation(substitution, j, i));
			}
		}
		
		for(auto i = 0; i <= programLen; ++i)
		{
			for(auto j = 0; j < instructionCount; ++j)
			{
				mutations.push_back(createMutation(addition, j, i));
			}
		}
	}
	
	return mutations;
}

Program MutationCache::mutateProgram(const Program& program, Mutation mutation)
{
	auto start = program.begin();
	auto programLen = program.size();
	auto instruction = getMutationInstruction(mutation);
	auto position = getMutationPosition(mutation);
	
	switch(getMutationType(mutation))
	{
		case addition:
			{
				Program result(start, start + position);
				result.push_back(instruction);
				result.insert(result.end(), start + position, program.end());
				return result;
			}
			break;
		case removal:
			{
				Program result(start, start + position);
				result.insert(result.end(), start + position + 1, program.end());
				return result;
			}
			break;
		case substitution:
			{
				Program result = program;
				result[position] = (result[position] + instruction) % instructionCount;
				return result;
			}
		default:
			throw;
	}
}