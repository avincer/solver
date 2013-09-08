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