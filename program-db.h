#include "types.h"

#include <vector>
#include <unordered_set>

class IProgramDB
{
	public:
		virtual void storeProgram(const Program& program) = 0;

		virtual bool findProgram(const Program& program) = 0;
};

class SetProgramDB : public IProgramDB
{
	private:
		//std::unordered_set<Program> programs;

	public:
		void storeProgram(const Program& program);

		bool findProgram(const Program& program);
};