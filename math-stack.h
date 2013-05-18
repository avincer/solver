#include <cmath>

#include "result.h"

class MathStack
{
	private:
		int maxSize;
		int pos;
		float* stack;
	
	public:
		MathStack(int maxSize)
		{
			this->maxSize = maxSize;
			stack = new float[maxSize];
			pos = 0;
		}
		
		// for the VM //
		void reset()
		{
			// if everything works as it's supposed to there is no need to 
			// reset whatever data may still be in the stack
			pos = 0;
		}
		
		void reset(float seed)
		{
			stack[0] = seed;
			pos = 1;
		}
		
		Result peek(float* x)
		{
			if(pos < 1) return StackUnderflow;
			*x = stack[pos - 1];
			return Ok;
		}
		
		Result pop(float* x)
		{
			if(pos < 1) return StackUnderflow;
			*x = stack[pos - 1];
			--pos;
			return Ok;
		}
		
		// stack functions //
		Result push(float x)
		{
			if(pos >= maxSize) return StackOverflow;
			stack[pos] = x;
			++pos;
			return Ok;
		}
		
		Result dup()
		{
			if(pos < 1) return StackUnderflow;
			if(pos >= maxSize) return StackOverflow;
			stack[pos] = stack[pos - 1];
			++pos;
			return Ok;
		}
		
		Result swap()
		{
			if(pos < 2) return StackUnderflow;
			float x = stack[pos - 2];
			stack[pos - 2] = stack[pos - 1];
			stack[pos - 1] = x;
			return Ok;
		}
		
		Result drop()
		{
			if(pos < 1) return StackUnderflow;
			--pos;
			return Ok;
		}
		
		// unary math //
		Result neg()
		{
			if(pos < 1) return StackUnderflow;
			stack[pos - 1] = -stack[pos - 1];
			return Ok;
		}
		
		Result inc()
		{
			if(pos < 1) return StackUnderflow;
			++stack[pos - 1];
			return Ok;
		}
		
		Result dec()
		{
			if(pos < 1) return StackUnderflow;
			--stack[pos - 1];
			return Ok;
		}
		
		Result trunc()
		{
			if(pos < 1) return StackUnderflow;
			stack[pos - 1] = (int)stack[pos - 1];
			return Ok;
		}
		
		// unary logic //
		Result lnot()
		{
			if(pos < 1) return StackUnderflow;
			stack[pos - 1] = !stack[pos - 1];
			return Ok;
		}
		
		// binary math //
		Result add()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] += stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result sub()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] -= stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result mul()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] *= stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result div()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] += stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result fmod()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = ::fmod(stack[pos - 2], stack[pos - 1]);
			--pos;
			return Ok;
		}
		
		// binary logic
		Result land()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] && stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result lor()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] || stack[pos - 1];
			--pos;
			return Ok;
		}
		
		// comparison
		Result eq()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] == stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result neq()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] != stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result gt()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] > stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result lt()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] < stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result gte()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] >= stack[pos - 1];
			--pos;
			return Ok;
		}
		
		Result lte()
		{
			if(pos < 2) return StackUnderflow;
			stack[pos - 2] = stack[pos - 2] <= stack[pos - 1];
			--pos;
			return Ok;
		}		
		
		~MathStack()
		{
			delete stack;
		}
};