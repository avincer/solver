#include "tree.h"

#include <iostream>

ProgramTree::ProgramTree(IRandom* random, 
			const std::vector<double>& initialChildWeights)
{
	this->random = random;
	
	// set node properties
	Node::instructionCount = initialChildWeights.size();
	Node::initialChildWeights = initialChildWeights;
	
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

int ProgramTree::chooseNextInstruction(Node* parent)
{
	// todo - consider caching this for speed
	double max = parent->sumChildWeights();
	
	double x = random->getDouble(max);
	double threshold = parent->getChildWeight(0);
	int i = 0;
	while(x > threshold)
	{
		++i;
		threshold += parent->getChildWeight(i);
	}
	return i;
}

Node* ProgramTree::allocateNode(int instruction, Node* parent)
{
	// todo - could probably do better with batch allocation
	return new Node(instruction, parent);
}

Node* ProgramTree::createNewNode(Node* parent)
{
	// choose instruction
	int i = chooseNextInstruction(parent);
	currentProgram.push_back(i);
	
	auto node = parent->getChild(i);
	if(node == nullptr)
	{
		// create and return new node
		return parent->createChild(i);
	}
	else
	{
		// keep exploring
		return createNewNode(node);
	}
}