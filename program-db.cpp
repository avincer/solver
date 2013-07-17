#include "program-db.h"

void SetProgramDB::storeProgram(const std::vector<int>& program)
{
	programs.emplace(program);
}

bool SetProgramDB::findProgram(const std::vector<int>& program)
{
	return programs.count(program);
}