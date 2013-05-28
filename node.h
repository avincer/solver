#include <vector>
#include <ostream>

// forward declaration of the Node class for use in NodeLink
class Node;

// link to and information about a child node
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

// program tree node
// each non-root node stores an instruction
// every node /represents/ a program
class Node
{
	private:
		// the total number of instructions / children
		// note: store this to avoid having to call std::vector.size() lots
		// make static to avoid storing it in each node
		static int instructionCount;
		
		// the instruction for this node
		// note: meaningless for the root node
		int instruction;
		
		// number of descendants
		int totalDescendantCount;
		
		// score for this program only
		double score;
		
		// score for all descendants
		double totalDescendantScore;
		
		// the parent of this node (null for the root node)
		Node* parent;
		
		// note: updateStats always called on a parent
		void updateStats(double newScore, int childInstruction, double childWeight);
		
		// setup steps common to both constructors
		void init(int instruction, Node* parent);
		
	public:
		// direct descendants of this node
		// note: public to allow program tree to choose children by weight
		std::vector<NodeLink> children;

		// ctor used by program tree
		Node(int instruction, Node* parent, 
			 const std::vector<double>& weights, 
			 const std::vector<double>& cumWeights);
		
		// ctor used in testing
		Node(int instruction, Node* parent, 
			 const std::vector<double>& initialChildWeights);
		
		// note: setScore is always called on a leaf node, 
		// i.e. weight === score
		void setScore(double score);
		
		void toXml(std::ostream& stream);
};