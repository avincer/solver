#include <vector>

typedef struct
{
	std::vector<int> instructions;
	void* node;
}
Program;

class IProgramFactory
{
	public:
		Program createNewProgram();
		
		void setProgramScore(Program program, double score);
		
		// todo - add functions to load and save state
};