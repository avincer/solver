#include "node.h"

void Node::updateStats(double newScore, int childInstruction, double childWeight)
{
	totalDescendantScore += newScore;
	totalDescendantCount++;
	
	children[childInstruction].weight = childWeight;
	
	// update cumulative weights
	// note: changing weight at position i affects all cumulative weights in position >= i
	double cumWeight = childInstruction ? children[childInstruction - 1].cumWeight : 0;
	for(int i = childInstruction; i < instructionCount; ++i)
	{
		cumWeight += children[i].weight;
		children[i].cumWeight = cumWeight;
	}
	
	if(parent != nullptr)
	{
		double myWeight = (score + totalDescendantScore) / (totalDescendantCount + 1);
		parent->updateStats(newScore, instruction, myWeight);
	}
}

void Node::init(int instruction, int instructionCount, Node* parent)
{
	this->instruction = instruction;
	this->parent = parent;
	score = 0;
	totalDescendantScore = 0;
	totalDescendantCount = 0;
	children.resize(instructionCount);
}

Node::Node(int instruction, Node* parent, 
		   const std::vector<double>& weights, 
		   const std::vector<double>& cumWeights)
{
	instructionCount = weights.size();
	init(instruction, instructionCount, parent);

	for(int i = 0; i < instructionCount; ++i)
	{
		children[i].node = nullptr;
		children[i].weight = weights[i];
		children[i].cumWeight = cumWeights[i];
	}
}

Node::Node(int instruction, Node* parent, 
		   const std::vector<double>& initialChildWeights)
{
	instructionCount = initialChildWeights.size();
	init(instruction, instructionCount, parent);
	
	for(int i = 0; i < instructionCount; ++i)
	{
		children[i].node = nullptr;
		children[i].weight = initialChildWeights[i];
		children[i].cumWeight = initialChildWeights[i];
		if(i)
		{
			children[i].cumWeight += children[i - 1].cumWeight;
		}
	}
}

void Node::setScore(double score)
{
	this->score = score;
	if(parent != nullptr) parent->updateStats(score, instruction, score);
}