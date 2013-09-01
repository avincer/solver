#include "program-factory.h"
#include "random.h"

#include <unordered_set>
#include <boost/functional/hash.hpp>

// c++ doesn't provide a hash function for std::vector of anything except bool
// this declaration adds a specialization of the hash function for Program
// note: this must be defined before the declaration of unordered_set
namespace std
{
	template <>
	struct hash<Program>
	{
		size_t operator()(const Program& program) const
		{
			return boost::hash_range(program.begin(), program.end());
		}
	};
}

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
		ProgramInfo programInfo;

	public:
		RandomFactory(IRandom* random, unsigned char instructionCount);

		std::string getName();
		
		const std::unordered_set<Program>& getPrograms() const;

		// returns a new (random) program
		ProgramInfo createNewProgram();
		
		bool addProgram(const Program& program);

		// no-op for this factory
		void recordProgramScore(const ProgramInfo& program) {}

		// review - do we need this?
		void toXml(std::ostream& stream) {}

		// nothing interesting to output
		void dumpProgramInformation(const Program& program) {}
};