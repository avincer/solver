#include "tree.h"

#include <iostream>

ProgramTree::ProgramTree(IRandom* random, 
			const std::vector<double>& initialChildWeights)
{
	this->random = random;
	this->initialChildWeights = initialChildWeights;
	
	// cache cumulative node weights for performance
	// note: this only works because all nodes have same weighting
	int instructionCount = initialChildWeights.size();
	double sum = 0;
	for(int i = 0; i < instructionCount; ++i)
	{
		sum += initialChildWeights[i];
		cumInitialWeights.push_back(sum);
	}
	
	// note: instruction for root node is ignored
	root = allocateNode(-1, nullptr);
}

Program ProgramTree::createNewProgram()
{
	// note: the intention here is to clear the vector but retain allocated memory
	// freeing and reallocating is a waste of time
	currentProgram.clear();
	
	Program p;
	p.node = createNewNode(root);
	p.instructions = currentProgram;
	return p;
}

void ProgramTree::recordProgramScore(Program program)
{
	auto node = (Node*)program.node;
	node->setScore(program.score);
}

void ProgramTree::toXml(std::ostream& stream)
{
	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	root->toXml(stream);
}

ProgramTree::~ProgramTree()
{
	cleanup(root);
}

int ProgramTree::chooseNextInstruction(Node* parent)
{
	double max = parent->children.back().cumWeight;
	double x = random->getDouble(max);
	int i = 0;
	while(x > parent->children[i].cumWeight)
	{
		++i;
	}
	return i;
}

Node* ProgramTree::allocateNode(int instruction, Node* parent)
{
	// todo - could probably do better with batch allocation
	return new Node(instruction, parent, initialChildWeights, cumInitialWeights);
}

Node* ProgramTree::createNewNode(Node* parent)
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

void ProgramTree::cleanup(Node* node)
{
	for(auto link: node->children)
	{
		if(link.node != nullptr) cleanup(link.node);
	}
	delete node;
}