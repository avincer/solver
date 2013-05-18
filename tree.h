#include "node.h"
#include "random.h"
#include <vector>
#include <iostream>

typedef struct
{
	Node* node;
	const int* instructions;
	int instructionCount;
}
Program;

class ProgramTree
{
	private:
		IRandom* random;
		const double* initialChildWeights;
		double cumInitialWeights[instructionCount];
		
		Node* root;
		std::vector<int> currentProgram;
	
	public:
		ProgramTree(IRandom* random, const double* initialChildWeights)
		{
			this->random = random;
			this->initialChildWeights = initialChildWeights;
			
			// cache cumulative node weights for performance
			// note: this only works because all nodes have same weighting
			double sum = 0;
			for(int i = 0; i < instructionCount; ++i)
			{
				sum += initialChildWeights[i];
				cumInitialWeights[i] = sum;
			}
			
			// note: instruction for root node is ignored
			root = allocateNode(-1, nullptr);
		}
		
		Program createNewProgram()
		{
			// note: the intention here is to clear the vector but retain allocated memory
			// freeing and reallocating is a waste of time
			currentProgram.clear();
			
			Program p;
			p.node = createNewNode(root);
			p.instructions = &currentProgram[0];
			p.instructionCount = currentProgram.size();
			return p;
		}
		
		~ProgramTree()
		{
			cleanup(root);
		}
		
		int chooseNextInstruction(Node* parent)
		{
			double max = parent->children[lastChild].cumWeight;
			double x = random->getDouble(max);
			int i = 0;
			while(x > parent->children[i].cumWeight)
			{
				++i;
			}
			return i;
		}
		
	private:
		Node* allocateNode(int instruction, Node* parent)
		{
			//return new Node(instruction, parent, initialChildWeights);
			return new Node(instruction, parent, initialChildWeights, cumInitialWeights);
		}
		
		Node* createNewNode(Node* parent)
		{
			// debug weights
			if(false)
			{
				for(auto link: parent->children)
				{
					std::cout << link.weight << " ";
				}
				std::cout << std::endl;
				std::cout << std::endl;
				
				for(auto link: parent->children)
				{
					std::cout << link.cumWeight << " ";
				}
				std::cout << std::endl;
				std::cout << std::endl;
			}
			
			// choose instruction
			int i = chooseNextInstruction(parent);
			currentProgram.push_back(i);
			
			if(parent->children[i].node == nullptr)
			{
				// create and return new node
				return parent->children[i].node = allocateNode(i, parent);
			}
			else
			{
				// keep exploring
				return createNewNode(parent->children[i].node);
			}
		}
		
		void cleanup(Node* node)
		{
			for(auto link: node->children)
			{
				if(link.node != nullptr) cleanup(link.node);
			}
			delete node;
		}
};