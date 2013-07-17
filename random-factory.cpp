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
ProgramInfo RandomFactory::createNewProgram()
{
	program.clear();
	do
	{
		program.push_back(random->getInt(instructionCount));
	}
	while(db->findProgram(program));
	db->storeProgram(program);
	ProgramInfo programInfo;
	programInfo.program = program;
	return programInfo;
}