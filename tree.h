#include "program-factory.h"
#include "node.h"
#include "random.h"

#include <vector>

typedef enum
{
	Random,
	Directed
}
SearchStrategy;

// program tree creates and stores programs
// Monte Carlo tree is used to generate new programs
// each new program is generated by appending an instruction to another
class ProgramTree : public IProgramFactory
{
	private:
		// random number source
		IRandom* random;
		
		// weights
		std::vector<double> initialChildWeights;
		
		// the root node (program of 0 length)
		Node* root;
		
		// storage for the current program (generated by walking the tree)
		std::vector<int> currentProgram;
		
		// strategy for generating programs
		SearchStrategy searchStrategy;
	
	public:
		// ctor
		// note: the instructionCount is taken to be the length of 
		// initialChildWeights
		ProgramTree(IRandom* random, 
					const std::vector<double>& initialChildWeights,
					SearchStrategy searchStrategy);
		
		std::string getName();
		
		// create and return a new program using MCTS
		Program createNewProgram();
		
		// set the score for a generated program, and update stats
		void recordProgramScore(Program program);
		
		void toXml(std::ostream& stream);
		
		void dumpProgramInformation(const std::vector<int>& program);
		
		~ProgramTree();
		
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