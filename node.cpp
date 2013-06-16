#include "node.h"

int Node::instructionCount = 0;

void Node::updateStats(double newScore, int childInstruction, double childWeight)
{
	totalDescendantScore += newScore;
	totalDescendantCount++;
	
	children[childInstruction].weight = childWeight;
	
	if(parent != nullptr)
	{
		double myWeight = (score + totalDescendantScore) / (totalDescendantCount + 1);
		parent->updateStats(newScore, instruction, myWeight);
	}
}

Node::Node(int instruction, Node* parent, 
		   const std::vector<double>& initialChildWeights)
{
	if(!instructionCount) instructionCount = initialChildWeights.size();
	
	this->instruction = instruction;
	this->parent = parent;
	score = 0;
	totalDescendantScore = 0;
	totalDescendantCount = 0;
	children.resize(instructionCount);
	
	for(int i = 0; i < instructionCount; ++i)
	{
		children[i].node = nullptr;
		children[i].weight = initialChildWeights[i];
	}
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
		if(children[i].node == nullptr)
		{
			stream << "<stub ";
			stream << "instruction=\"" << i << "\" ";
			stream << "weight=\"" << children[i].weight << "\"";
			stream << "/>" << std::endl;
		}
		else
		{
			children[i].node->toXml(stream);
		}
	}
	stream << "</node>" << std::endl;
}