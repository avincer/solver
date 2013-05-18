#include <cstring>

#include "vm.h"

typedef bool (VM::*Handler)();

VM::VM(int stackSize, int memorySize) : stack(stackSize)
{
	memory = new float[memorySize];
	this->memorySize = memorySize;
	
	// set handlers for instructions
	handlers[dup] = &MathStack::dup;
	handlers[swap] = &MathStack::swap;
	handlers[drop] = &MathStack::drop;
	
	handlers[neg] = &MathStack::neg;
	handlers[inc] = &MathStack::inc;
	handlers[dec] = &MathStack::dec;
	handlers[truncate] = &MathStack::trunc;
	
	handlers[lnot] = &MathStack::lnot;
	
	handlers[add] = &MathStack::add;
	handlers[sub] = &MathStack::sub;
	handlers[mul] = &MathStack::mul;
	handlers[divide] = &MathStack::div;
	handlers[fmodulus] = &MathStack::fmod;
	
	handlers[land] = &MathStack::land;
	handlers[lor] = &MathStack::lor;
	
	handlers[eq] = &MathStack::eq;
	handlers[neq] = &MathStack::neq;
	handlers[gt] = &MathStack::gt;
	handlers[lt] = &MathStack::lt;
	handlers[gte] = &MathStack::gte;
	handlers[lte] = &MathStack::lte;
}

void VM::loadProgram(Instruction* program, int programLen, int maxOps)
{
	this->program = program;
	this->programLen = programLen;
	this->maxOps = maxOps;
	
	// memory is only reset on load
	memset(memory, 0, memorySize * sizeof(float));
}

// todo - return reason for failure
Result VM::run(float seed, float* output)
{
	if(!program) throw "No program loaded!";
	
	int pos = 0;
	int opCount = 0;
	Result result = Ok;
	float target, condition;
	
	// stack is reset at the start of each run
	stack.reset(seed);
	
	do
	{
		Instruction instruction = program[pos];
		if(instruction >= pushNeg1 && instruction <= push10)
		{
			result = stack.push(instruction - push0);
			++pos;
		}
		else
		{
			switch(instruction)
			{
				case jif:
					result = stack.pop(&target) && stack.pop(&condition) ? Ok : StackUnderflow;
					if(result && condition) pos += (int)target;
					break;
				case jmp:
					result = stack.pop(&target);
					if(result) pos += (int)target;
					break;
				case ret:
					pos = -1;
					break;
					
				case get:
				case put:
					result = stack.pop(&target);
					if(result)
					{
						int addr = (int)target;
						// wrap to [0, memorySize) (don't forget negative numbers!)
						addr = ((addr % memorySize) + memorySize) % memorySize;
						
						if(instruction == get)
						{
							result = stack.push(memory[addr]);
						}
						else
						{
							result = stack.pop(&target);
							if(result) memory[addr] = target;
						}
					}
					break;
					
				
				default:
					auto handler = handlers[instruction];
					if(handler)
					{
						result = (stack.*handler)();
					}
					else
					{
						throw "Something went wrong - unknown instruction!";
					}
			}
			if(instruction != jif && instruction != jmp) ++pos;
		}
		++opCount;
	}
	while(result && pos >= 0 && pos < programLen && opCount < maxOps);
	
	if(result && opCount < maxOps)
	{
		// program terminated normally - try and get result
		return stack.peek(output);
	}
	
	// failed
	return result ? ExceededMaxOpCount : result;
}

VM::~VM()
{
	delete memory;
}