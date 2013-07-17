#include "program-factory.h"
#include "random.h"
#include "program-db.h"

#include <memory>

class RandomFactory : public IProgramFactory
{
	private:
		IRandom* random;
		int instructionCount;
		std::unique_ptr<IProgramDB> db;
		Program program;

	public:
		RandomFactory(IRandom* random, int instructionCount);

		std::string getName();

		// returns a new program
		ProgramInfo createNewProgram();

		// no-op for this factory
		void recordProgramScore(ProgramInfo program) {}

		// review - do we need this?
		void toXml(std::ostream& stream) {}

		// nothing interesting to output
		void dumpProgramInformation(const Program& program) {}
};