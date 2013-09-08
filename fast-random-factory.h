#include "program-factory.h"
#include "random.h"

#include <cmath>

typedef struct
{
	double permutations;
	double pNotProgram; // probability of not having generated a particular program
	double pAllPrograms; // probability of having generated all programs
}
Probabilities;

class FastRandomFactory : public IProgramFactory
{
	private:
		IRandom* random;
		unsigned char instructionCount;
		
		std::vector<Probabilities> probs;
		Program program;
		
	public:
		FastRandomFactory(IRandom* random, unsigned char instructionCount)
		{
			this->random = random;
			this->instructionCount = instructionCount;
			
			Probabilities prob;
			prob.permutations = 1;
			prob.pNotProgram = 1;
			prob.pAllPrograms = 0;
			probs.push_back(prob);
			
			while(1)
			{
				// generate stats until we haven't the precision to do the math!
				prob.permutations *= instructionCount;
				if((prob.permutations - 1.0) / prob.permutations == 1.0) return;
				
				probs.push_back(prob);
			}
		}
		
		std::string getName() { return "FastRandomFactory"; }
		
		// xxx - this method is not thread safe!
		const Program& createNewProgram()
		{
			// choose length
			auto len = 1;
			for(; (len < probs.size() && (probs[len].pAllPrograms > 0.1 || random->maybe(probs[len].pAllPrograms))); ++len);
			
			// update probabilities
			probs[len].pNotProgram *= (probs[len].permutations - 1.0) / probs[len].permutations;
			probs[len].pAllPrograms = pow(1.0 - probs[len].pNotProgram, probs[len].permutations);
			
			program.clear();
			program.reserve(len);
			for(auto i = 0; i < len; ++i)
			{
				program.push_back(random->getInt(instructionCount));
			}
			
			return program;
		}
		
		// no-op for this factory
		void recordProgramScore(const Program& program, double score) {}
		
		// nothing interesting to output
		void dumpProgramInformation(const Program& program) {}
};