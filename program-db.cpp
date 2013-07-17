#include "program-db.h"

void SetProgramDB::storeProgram(const Program& program)
{
	programs.emplace(program);
}

bool SetProgramDB::findProgram(const Program& program)
{
	return programs.count(program);
}