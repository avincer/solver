#include <vector>
#include <map>
#include <ostream>
#include <memory>

// forward declaration of the Node class for use in NodeLink
class Node;

// link to and information about a child node
typedef struct
{
	// the child node, which may NOT be null
	std::unique_ptr<Node> node;
	
	// weight for this child node
	double weight;
}
NodeLink;

// program tree node
// each non-root node stores an instruction
// every node /represents/ a program
class Node
{
	public:
		// the total number of instructions / children
		// note: store this to avoid having to call std::vector.size() lots
		// make static to avoid storing it in each node
		static int instructionCount;
		
		// initial weights for children of this node
		// note: static because all nodes use the same weights
		static std::vector<double> initialChildWeights;
	
	private:
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
		
		// direct descendants of this node
		std::map<int, NodeLink> children;
		
		// note: updateStats always called on a parent
		void updateStats(double newScore, int childInstruction, double childWeight);
		
	public:
		Node(int instruction, Node* parent);
		
		Node* createChild(int instruction);
		
		// returns the child node or null if it doesn't exist
		Node* getChild(int instruction);
		
		// returns the weight of the corresponding child node, if it exists,
		// else returns the initial child weight
		double getChildWeight(int instruction);
		
		// sums getChildWeight over all child nodes, explored or not
		double sumChildWeights();
		
		// note: setScore is always called on a leaf node, 
		// i.e. weight === score
		void setScore(double score);
		
		void toXml(std::ostream& stream);
};