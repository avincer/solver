#include <cmath>
#include <iostream>
#include <iomanip>

#include "math-stack.h"

MathStack::MathStack(int maxSize)
{
	this->maxSize = maxSize;
	stack = new float[maxSize];
	pos = 0;
}

// for the VM //
void MathStack::reset()
{
	// if everything works as it's supposed to there is no need to 
	// reset whatever data may still be in the stack
	pos = 0;
}

void MathStack::reset(float seed)
{
	stack[0] = seed;
	pos = 1;
}

void MathStack::dump(const char* prefix)
{
	std::cout << prefix << "\t[ " << std::setprecision(3);
	for(int i = 0; i < pos; ++i)
	{
		std::cout << stack[i] << " ";
	}
	std::cout << "]" << std::endl;
}

Result MathStack::peek(float* x)
{
	if(pos < 1) return StackUnderflow;
	*x = stack[pos - 1];
	return Ok;
}

Result MathStack::pop(float* x)
{
	if(pos < 1) return StackUnderflow;
	*x = stack[pos - 1];
	--pos;
	return Ok;
}

// stack functions //
Result MathStack::push(float x)
{
	if(pos >= maxSize) return StackOverflow;
	stack[pos] = x;
	++pos;
	return Ok;
}

Result MathStack::dup()
{
	if(pos < 1) return StackUnderflow;
	if(pos >= maxSize) return StackOverflow;
	stack[pos] = stack[pos - 1];
	++pos;
	return Ok;
}

Result MathStack::swap()
{
	if(pos < 2) return StackUnderflow;
	float x = stack[pos - 2];
	stack[pos - 2] = stack[pos - 1];
	stack[pos - 1] = x;
	return Ok;
}

Result MathStack::drop()
{
	if(pos < 1) return StackUnderflow;
	--pos;
	return Ok;
}

// unary math //
Result MathStack::neg()
{
	if(pos < 1) return StackUnderflow;
	stack[pos - 1] = -stack[pos - 1];
	return Ok;
}

Result MathStack::inc()
{
	if(pos < 1) return StackUnderflow;
	++stack[pos - 1];
	return Ok;
}

Result MathStack::dec()
{
	if(pos < 1) return StackUnderflow;
	--stack[pos - 1];
	return Ok;
}

Result MathStack::trunc()
{
	if(pos < 1) return StackUnderflow;
	stack[pos - 1] = (int)stack[pos - 1];
	return Ok;
}

// unary logic //
Result MathStack::lnot()
{
	if(pos < 1) return StackUnderflow;
	stack[pos - 1] = !stack[pos - 1];
	return Ok;
}

// binary math //
Result MathStack::add()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] += stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::sub()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] -= stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::mul()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] *= stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::div()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] += stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::fmod()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = ::fmod(stack[pos - 2], stack[pos - 1]);
	--pos;
	return Ok;
}

// binary logic
Result MathStack::land()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] && stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::lor()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] || stack[pos - 1];
	--pos;
	return Ok;
}

// comparison
Result MathStack::eq()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] == stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::neq()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] != stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::gt()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] > stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::lt()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] < stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::gte()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] >= stack[pos - 1];
	--pos;
	return Ok;
}

Result MathStack::lte()
{
	if(pos < 2) return StackUnderflow;
	stack[pos - 2] = stack[pos - 2] <= stack[pos - 1];
	--pos;
	return Ok;
}		

MathStack::~MathStack()
{
	delete stack;
}