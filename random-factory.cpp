#include "random-factory.h"

RandomFactory::RandomFactory(IRandom* random, int instructionCount)
{
	this->random = random;
	this->instructionCount = instructionCount;
	db.reset(new SetProgramDB());
}

std::string RandomFactory::getName()
{
	return "RandomFactory";
}

// returns a new program
Program RandomFactory::createNewProgram()
{
	instructions.clear();
	do
	{
		instructions.push_back(random->getInt(instructionCount));
	}
	while(db->findProgram(instructions));
	db->storeProgram(instructions);
	Program program;
	program.instructions = instructions;
	return program;
}