#include "tree.h"

#include <iostream>

ProgramTree::ProgramTree(IRandom* random, 
			const std::vector<double>& initialChildWeights,
			SearchStrategy searchStrategy)
{
	this->random = random;
	this->initialChildWeights = initialChildWeights;
	this->searchStrategy = searchStrategy;
	
	// note: instruction for root node is ignored
	root = allocateNode(-1, nullptr);
}

std::string ProgramTree::getName()
{
	return searchStrategy == Random ? 
		"ProgramTree(random search)" : "ProgramTree(directed search)";
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

void ProgramTree::dumpProgramInformation(const std::vector<int>& program)
{
	double chance = 1;
	auto node = root;
	for(int i = 0; i < program.size() && node != nullptr; ++i)
	{
		// print sub-program path
		for(int j = 0; j <= i; ++j)
		{
			std::cout << "/" << program[j];
		}
		
		auto link = node->children[program[i]];
		bool exists = link.node != nullptr;
		if(exists)
		{
			std::cout << " exists with score " << link.node->score;
		}
		else
		{
			std::cout << " unexplored";
		}
		
		double totalWeight = 0;
		for(int c = 0; c < node->children.size(); ++c)
		{
			totalWeight += node->children[c].weight;
		}
		
		double weight = link.weight;
		double fractionalWeight = weight / totalWeight;
		chance *= fractionalWeight;
		
		std::cout << " weight " << weight << " / " << totalWeight;
		std::cout << " (" << fractionalWeight * 100 << "%)";
		std::cout << " chance " << (long)(1.0 / chance) << ":1" << std::endl;
		
		node = link.node;
	}
	std::cout << std::endl;
}

ProgramTree::~ProgramTree()
{
	cleanup(root);
}

int ProgramTree::chooseNextInstruction(Node* parent)
{
	if(searchStrategy == Random)
	{
		return random->getInt(initialChildWeights.size());
	}
	else
	{
		// xxx - this should probably be cached!
		double max = 0;
		for(auto child : parent->children)
		{
			max += child.weight;
		}
		
		double x = random->getDouble(max);
		double threshold = parent->children[0].weight;
		int i = 0;
		while(x > threshold)
		{
			++i;
			threshold += parent->children[i].weight;
		}
		return i;
	}
}

Node* ProgramTree::allocateNode(int instruction, Node* parent)
{
	// todo - could probably do better with batch allocation
	return new Node(instruction, parent, initialChildWeights);
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