#include <cassert>
#include <iostream>
#include <algorithm>

// pure evil, but a rather elegant way of testing things :)
#define private public

#include "top-factory.h"

MutationCache mutationCache(7);

const Program input { 1, 2, 3, 4, 5 };

void testMutation(size_t mutation, Program target)
{
	assert(mutationCache.mutateProgram(input, mutation) == target);
}

void testMutationCache()
{
	// addition tests
	testMutation(createMutation(addition, 6, 0), { 6, 1, 2, 3, 4, 5 });
	testMutation(createMutation(addition, 0, 2), { 1, 2, 0, 3, 4, 5 });
	testMutation(createMutation(addition, 0, 5), { 1, 2, 3, 4, 5, 0 });
	
	// removal tests
	testMutation(createMutation(removal, 0, 0), { 2, 3, 4, 5 });
	testMutation(createMutation(removal, 0, 3), { 1, 2, 3, 5 });
	testMutation(createMutation(removal, 0, 4), { 1, 2, 3, 4 });
	
	// substitution
	testMutation(createMutation(substitution, 3, 1), {1, 5, 3, 4, 5});
	testMutation(createMutation(substitution, 6, 2), {1, 2, 2, 4, 5});
	
	// get mutations
	std::vector<Mutation> expectedMutations
	{
		createMutation(removal, 0, 0),
		createMutation(substitution, 1, 0),
		createMutation(substitution, 2, 0),
		createMutation(substitution, 3, 0),
		createMutation(substitution, 4, 0),
		createMutation(substitution, 5, 0),
		createMutation(substitution, 6, 0),
		
		createMutation(removal, 0, 1),
		createMutation(substitution, 1, 1),
		createMutation(substitution, 2, 1),
		createMutation(substitution, 3, 1),
		createMutation(substitution, 4, 1),
		createMutation(substitution, 5, 1),
		createMutation(substitution, 6, 1),
		
		createMutation(addition, 0, 0),
		createMutation(addition, 1, 0),
		createMutation(addition, 2, 0),
		createMutation(addition, 3, 0),
		createMutation(addition, 4, 0),
		createMutation(addition, 5, 0),
		createMutation(addition, 6, 0),
		
		createMutation(addition, 0, 1),
		createMutation(addition, 1, 1),
		createMutation(addition, 2, 1),
		createMutation(addition, 3, 1),
		createMutation(addition, 4, 1),
		createMutation(addition, 5, 1),
		createMutation(addition, 6, 1),
		
		createMutation(addition, 0, 2),
		createMutation(addition, 1, 2),
		createMutation(addition, 2, 2),
		createMutation(addition, 3, 2),
		createMutation(addition, 4, 2),
		createMutation(addition, 5, 2),
		createMutation(addition, 6, 2),
	};
	
	auto actualMutations = mutationCache.getMutations(2);
	
	assert(actualMutations == expectedMutations);
}

void testRandomFactory()
{
	CStdRandom random;
	RandomFactory randomFactory(&random, 10);
	
	// must not be able to add the same program twice
	Program program { 1, 2, 3 };
	assert(randomFactory.addProgram(program) == true);
	assert(randomFactory.addProgram(program) == false);
	
	// manually adding programs should update stats
	assert(randomFactory.stats[1].remaining == 10);
	randomFactory.addProgram( { 0 } );
	randomFactory.addProgram( { 1 } );
	randomFactory.addProgram( { 2 } );
	randomFactory.addProgram( { 3 } );
	randomFactory.addProgram( { 4 } );
	randomFactory.addProgram( { 5 } );
	randomFactory.addProgram( { 6 } );
	randomFactory.addProgram( { 7 } );
	randomFactory.addProgram( { 8 } );
	randomFactory.addProgram( { 9 } );
	assert(!randomFactory.stats[1].remaining);
	
	auto programInfo = randomFactory.createNewProgram();
	assert(programInfo.program != program);
}

void testTopFactory()
{
	CStdRandom random;
	TopFactory topFactory(&random, 10, 3, 0.5);
	
	// test mutations
	std::vector<Mutation> input { 1, 2, 3, 4, 5 };
	std::vector<Mutation> output;
	for(auto i = 0; i < 5; ++i)
	{
		output.push_back(topFactory.popRandomMutation(input));
	}
	assert(!input.size());
	
	if(input == output)
	{
		std::cout << "Warning: input and output sequences equal. Re-run test!" << std::endl;
	}
	
	assert(std::find(output.begin(), output.end(), 1) != output.end());
	assert(std::find(output.begin(), output.end(), 2) != output.end());
	assert(std::find(output.begin(), output.end(), 3) != output.end());
	assert(std::find(output.begin(), output.end(), 4) != output.end());
	assert(std::find(output.begin(), output.end(), 5) != output.end());
	
	// test recording program score
	ProgramInfo pi;
	pi.score = 4;
	topFactory.recordProgramScore(pi);
	
	pi.score = 3;
	topFactory.recordProgramScore(pi);
	
	pi.program.push_back(1);
	topFactory.recordProgramScore(pi);
	
	assert(topFactory.topPrograms[0].score == 4);
	assert(topFactory.topPrograms[1].score == 3);
	assert(topFactory.topPrograms[2].score == 3 && topFactory.topPrograms[2].program.size() == 1);
	
	// adding a low scoring program should do nothing
	pi.program.clear();
	pi.score = 1;
	topFactory.recordProgramScore(pi);
	assert(topFactory.topPrograms.size() == 3);
	assert(topFactory.topPrograms[0].score == 4);
	assert(topFactory.topPrograms[1].score == 3);
	assert(topFactory.topPrograms[2].score == 3 && topFactory.topPrograms[2].program.size() == 1);
	
	// adding a high scoring program should push others down
	pi.score = 3.5;
	topFactory.recordProgramScore(pi);
	assert(topFactory.topPrograms.size() == 3);
	assert(topFactory.topPrograms[0].score == 4);
	assert(topFactory.topPrograms[1].score == 3.5);
	assert(topFactory.topPrograms[2].score == 3 && topFactory.topPrograms[2].program.size() == 0);
}

int main()
{
	testMutationCache();
	testRandomFactory();
	testTopFactory();
	
	std::cout << "It works!" << std::endl;
	
	return 0;
}
