#include "program-factory.h"
#include "program-hash.h"
#include "random.h"

#include <unordered_set>

typedef struct
{
	size_t count;
	size_t remaining;
}
ProgramStats;
// review name

class RandomFactory : public IProgramFactory
{
	private:
		IRandom* random;
		const unsigned char instructionCount;
		
		std::vector<ProgramStats> stats;
		size_t statCount;
		
		std::unordered_set<Program> programs;
		Program program;

	public:
		RandomFactory(IRandom* random, unsigned char instructionCount);

		std::string getName();
		
		const std::unordered_set<Program>& getPrograms() const;

		// returns a new (random) program
		const Program& createNewProgram();
		
		const Program* addProgram(const Program& program);

		// no-op for this factory
		void recordProgramScore(const Program& program, double score) {}

		// nothing interesting to output
		void dumpProgramInformation(const Program& program) {}
};