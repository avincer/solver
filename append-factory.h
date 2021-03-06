#include "program-factory.h"
#include "node.h"
#include "random.h"

#include <vector>

// Monte Carlo tree search is used to generate new programs
// each new program is generated by appending an instruction to an existing program
class AppendFactory : public IProgramFactory
{
	private:
		// random number source
		IRandom* random;
		
		// weights
		std::vector<double> initialChildWeights;
		
		// the root node (program of 0 length)
		Node* root;
		
		// storage for the current program (generated by walking the tree)
		Program currentProgram;
	
	public:
		// ctor
		// note: the instructionCount is taken to be the length of 
		// initialChildWeights
		AppendFactory(IRandom* random,
					const std::vector<double>& initialChildWeights);
		
		std::string getName();
		
		// create and return a new program using MCTS
		ProgramInfo createNewProgram();
		
		// set the score for a generated program, and update stats
		void recordProgramScore(const ProgramInfo& program);
		
		void toXml(std::ostream& stream);
		
		void dumpProgramInformation(const Program& program);
		
		~AppendFactory();
		
		// choose an instruction to consider at weighted random
		// note: this is public for testing
		int chooseNextInstruction(Node* parent);
		
	private:
		// allocate a node (memory allocation only)
		Node* allocateNode(int instruction, Node* parent);
		
		// create a new node and set properties
		Node* createNewNode(Node* parent);
		
		// free memory
		void cleanup(Node* node);
};