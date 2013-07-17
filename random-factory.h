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
		std::vector<int> instructions;

	public:
		RandomFactory(IRandom* random, int instructionCount);

		std::string getName();

		// returns a new program
		Program createNewProgram();

		// no-op for this factory
		void recordProgramScore(Program program) {}

		// review - do we need this?
		void toXml(std::ostream& stream) {}

		// nothing interesting to output
		void dumpProgramInformation(const std::vector<int>& program) {}
};