#include "config.h"

#define lastChild (instructionCount - 1)

class Node;

typedef struct
{
	// the child node, which may be null (unexplored)
	Node* node;
	
	// weight for this child node even if null
	double weight;
	
	// cumulative weight added up across children (used in random selection)
	double cumWeight;
}
NodeLink;

class Node
{
	private:
		// note: instruction is meaningless for the root node
		int instruction;
		
		// score for this program only
		double score;
		
		// score for all descendants
		double totalDescendantScore;
		
		// number of descendants
		int totalDescendantCount;
		
		Node* parent;
		
		// updateStats always called on a parent
		void updateStats(double newScore, int childInstruction, double childWeight)
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
			
			if(parent)
			{
				double myWeight = (score + totalDescendantScore) / (totalDescendantCount + 1);
				parent->updateStats(newScore, instruction, myWeight);
			}
		}
		
		void init(int instruction, Node* parent)
		{
			this->instruction = instruction;
			this->parent = parent;
			score = 0;
			totalDescendantScore = 0;
			totalDescendantCount = 0;
		}
		
	public:
		// public so tree can select nodes according to weight
		NodeLink children[instructionCount];

		Node(int instruction, Node* parent, const double* weights, const double* cumWeights)
		{
			init(instruction, parent);
			
			for(int i = 0; i < instructionCount; ++i)
			{
				children[i].node = nullptr;
				children[i].weight = weights[i];
				children[i].cumWeight = cumWeights[i];
			}
		}
		
		Node(int instruction, Node* parent, const double* initialChildWeights)
		{
			init(instruction, parent);
			
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
		
		// setScore is always called on a leaf node, i.e. weight === score
		void setScore(double score)
		{
			this->score = score;
			if(parent) parent->updateStats(score, instruction, score);
		}
};