#include "node.h"

int Node::instructionCount = 0;

std::vector<double> Node::initialChildWeights;

void Node::updateStats(double newScore, int childInstruction, double childWeight)
{
	totalDescendantScore += newScore;
	totalDescendantCount++;
	
	children.at(childInstruction).weight = childWeight;
	
	if(parent != nullptr)
	{
		double myWeight = (score + totalDescendantScore) / (totalDescendantCount + 1);
		parent->updateStats(newScore, instruction, myWeight);
	}
}

Node::Node(int instruction, Node* parent)
{
	this->instruction = instruction;
	this->parent = parent;
	score = 0;
	totalDescendantScore = 0;
	totalDescendantCount = 0;
}

Node* Node::createChild(int instruction)
{
	children[instruction].weight = 0;
	Node* node = new Node(instruction, this);
	children[instruction].node.reset(node);
	return node;
}

Node* Node::getChild(int instruction)
{
	// xxx - fix use of unique_ptr::get
	return children.count(instruction) ? children[instruction].node.get() : nullptr;
}

double Node::getChildWeight(int instruction)
{
	return children.count(instruction) ? children[instruction].weight : initialChildWeights[instruction];
}

double Node::sumChildWeights()
{
	double sum = 0;
	for(int i = 0; i < instructionCount; ++i)
	{
		sum += getChildWeight(i);
	}
	return sum;
}

void Node::setScore(double score)
{
	this->score = score;
	if(parent != nullptr) parent->updateStats(score, instruction, score);
}

void Node::toXml(std::ostream& stream)
{
	stream << "<node ";
	stream << "instruction=\"" << instruction << "\" ";
	stream << "score=\"" << score << "\" ";
	stream << "totalDescendantScore=\"" << totalDescendantScore << "\" ";
	stream << "totalDescendantCount=\"" << totalDescendantCount << "\"";
	stream << ">" << std::endl;
	for(int i = 0; i < instructionCount; ++i)
	{
		if(!children.count(i))
		{
			stream << "<stub ";
			stream << "instruction=\"" << i << "\" ";
			stream << "weight=\"" << initialChildWeights[i] << "\"";
			stream << "/>" << std::endl;
		}
		else
		{
			children[i].node->toXml(stream);
		}
	}
	stream << "</node>" << std::endl;
}