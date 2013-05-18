#include "result.h"

class MathStack
{
	private:
		int maxSize;
		int pos;
		float* stack;
	
	public:
		MathStack(int maxSize);
		
		// for the VM //
		void reset();
		
		void reset(float seed);
		
		void dump(const char* prefix);
		
		Result peek(float* x);
		
		Result pop(float* x);
		
		// stack functions //
		Result push(float x);
		
		Result dup();
		
		Result swap();
		
		Result drop();
		
		// unary math //
		Result neg();
		
		Result inc();
		
		Result dec();
		
		Result trunc();
		
		// unary logic //
		Result lnot();
		
		// binary math //
		Result add();
		
		Result sub();
		
		Result mul();
		
		Result div();
		
		Result fmod();
		
		// binary logic
		Result land();
		
		Result lor();
		
		// comparison
		Result eq();
		
		Result neq();
		
		Result gt();
		
		Result lt();
		
		Result gte();
		
		Result lte();
		
		~MathStack();
};