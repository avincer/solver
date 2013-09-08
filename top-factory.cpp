#include "top-factory.h"

#include <sstream>

TopFactory::TopFactory(IRandom* random, unsigned char instructionCount,
					   size_t maxTopPrograms, double explorationChance) :
	random(random),
	instructionCount(instructionCount),
	maxTopPrograms(maxTopPrograms),
	explorationChance(explorationChance),
	randomFactory(new RandomFactory(random, instructionCount)),
	mutationCache(instructionCount)
{
	topPrograms.reserve(maxTopPrograms);
}

std::string TopFactory::getName()
{
	std::ostringstream name;
	name << "TopFactory(maxTopPrograms = " << maxTopPrograms << 
		", explorationChance = " << explorationChance << ")";
	return name.str();
}

const RandomFactory* TopFactory::getRandomFactory()
{
	return randomFactory.get();
}

const Program& TopFactory::createNewProgram()
{
	if(topPrograms.size() < maxTopPrograms || random->maybe(explorationChance))
	{
		// new random program
		return randomFactory->createNewProgram();
	}
	else
	{
		// mutate existing program
		while(1)
		{
			auto pos = random->getInt(maxTopPrograms);
			TopProgram& topProgram = topPrograms[pos];
			if(!topProgram.remainingMutations.size())
			{
				// first use of this program - store mutations
				topProgram.remainingMutations = mutationCache.getMutations(topProgram.program.size());
			}
			
			auto mutation = popRandomMutation(topProgram.remainingMutations);
			
			auto program = mutationCache.mutateProgram(topProgram.program, mutation);
			
			if(!topProgram.remainingMutations.size())
			{
				// no more mutations to explore, remove program from list
				topPrograms.erase(topPrograms.begin() + pos);
			}
			
			auto storedProgram = randomFactory->addProgram(program);
			if(storedProgram) return *storedProgram;
		}
	}
}

void TopFactory::recordProgramScore(const Program& program, double score)
{
	if(topPrograms.size() < maxTopPrograms || 
		score > topPrograms.back().score)
	{
		if(topPrograms.size() == maxTopPrograms)
		{
			// remove worst program to make room
			topPrograms.pop_back();
		}
		
		TopProgram topProgram;
		topProgram.score = score;
		topProgram.program = program;
		
		// xxx: linear search over ordered list!
		auto it = topPrograms.begin();
		for(it = topPrograms.begin(); 
			it != topPrograms.end() && it->score >= score;
			++it);
		topPrograms.insert(it, topProgram);
	}
}

Mutation TopFactory::popRandomMutation(std::vector<Mutation>& mutations)
{
	auto pos = random->getInt(mutations.size());
	auto mutation = mutations[pos];
	mutations[pos] = mutations.back();
	mutations.pop_back();
	return mutation;
}