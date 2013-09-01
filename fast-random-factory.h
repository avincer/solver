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
		ProgramInfo programInfo;
		
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
		
		ProgramInfo createNewProgram()
		{
			// choose length
			auto len = 1;
			for(; (len < probs.size() && (probs[len].pAllPrograms > 0.1 || random->maybe(probs[len].pAllPrograms))); ++len);
			
			// update probabilities
			probs[len].pNotProgram *= (probs[len].permutations - 1.0) / probs[len].permutations;
			probs[len].pAllPrograms = pow(1.0 - probs[len].pNotProgram, probs[len].permutations);
			
			programInfo.program.clear();
			programInfo.program.reserve(len);
			for(auto i = 0; i < len; ++i)
			{
				programInfo.program.push_back(random->getInt(instructionCount));
			}
			
			return programInfo;
		}
		
		// no-op for this factory
		void recordProgramScore(const ProgramInfo& program) {}
		
		// review - do we need this?
		void toXml(std::ostream& stream) {}
		
		// nothing interesting to output
		void dumpProgramInformation(const Program& program) {}
};