#include "random-factory.h"
#include "mutation-cache.h"

#include <memory>

typedef struct
{
	double score;
	
	Program program;
	
	std::vector<Mutation> remainingMutations;
}
TopProgram;

class TopFactory : public IProgramFactory
{
	private:
		IRandom* random;
		const unsigned char instructionCount;
		const size_t maxTopPrograms;
		const double explorationChance;
		
		std::unique_ptr<RandomFactory> randomFactory;
		std::vector<TopProgram> topPrograms;
		MutationCache mutationCache;
		
	public:
		TopFactory(IRandom* random, unsigned char instructionCount,
				   size_t maxTopPrograms, double explorationChance);
		
		std::string getName();
		
		ProgramInfo createNewProgram();
		
		void recordProgramScore(const ProgramInfo& program);
		
		Mutation popRandomMutation(std::vector<Mutation>& mutations);
	
		// todo: remove this method
		void toXml(std::ostream& stream) {}
		
		// todo: write this if required
		void dumpProgramInformation(const Program& program) {}
};