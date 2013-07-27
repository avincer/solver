#include "append-factory.h"

#include <iostream>

AppendFactory::AppendFactory(IRandom* random, 
			const std::vector<double>& initialChildWeights)
{
	this->random = random;
	this->initialChildWeights = initialChildWeights;
	
	// note: instruction for root node is ignored
	root = allocateNode(-1, nullptr);
}

std::string AppendFactory::getName()
{
	return "AppendFactory";
}

ProgramInfo AppendFactory::createNewProgram()
{
	// note: the intention here is to clear the vector but retain allocated memory
	// freeing and reallocating is a waste of time
	currentProgram.clear();
	
	ProgramInfo p;
	p.node = createNewNode(root);
	p.program = currentProgram;
	return p;
}

void AppendFactory::recordProgramScore(ProgramInfo program)
{
	auto node = (Node*)program.node;
	node->setScore(program.score);
}

void AppendFactory::toXml(std::ostream& stream)
{
	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	root->toXml(stream);
}

void AppendFactory::dumpProgramInformation(const Program& program)
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

AppendFactory::~AppendFactory()
{
	cleanup(root);
}

int AppendFactory::chooseNextInstruction(Node* parent)
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

Node* AppendFactory::allocateNode(int instruction, Node* parent)
{
	// todo - could probably do better with batch allocation
	return new Node(instruction, parent, initialChildWeights);
}

Node* AppendFactory::createNewNode(Node* parent)
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

void AppendFactory::cleanup(Node* node)
{
	for(auto link: node->children)
	{
		if(link.node != nullptr) cleanup(link.node);
	}
	delete node;
}