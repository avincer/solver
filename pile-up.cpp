#include <cstring>
#include <iostream>

#include "pile-up.h"

typedef bool (PileUp::*Handler)();

PileUp::PileUp(int stackSize, int memorySize) : stack(stackSize)
{
	memory = new float[memorySize];
	this->memorySize = memorySize;
	
	debugMode = None;
	
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

void PileUp::setDebugMode(DebugMode flags)
{
	debugMode = flags;
}

int PileUp::getInstructionCount()
{
	return InstructionCount;
}

void PileUp::loadProgram(const std::vector<int>& program)
{
	this->program = program;
	
	// memory is only reset on load
	memset(memory, 0, memorySize * sizeof(float));
}

bool PileUp::run(float seed, float* output, int maxOps)
{
	if(!program.size()) throw "No program loaded!";
	
	int pos = 0;
	int opCount = 0;
	float target, condition;
	
	// stack is reset at the start of each run
	stack.reset(seed);
	
	if(debugMode & DumpStackOnEntry) stack.dump("(entry)");
	
	result = Ok;
	
	do
	{
		Instruction instruction = (Instruction)program[pos];
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
					result = stack.pop(&target);
					if(result == Ok)
					{
						result = stack.pop(&condition);
						if(result == Ok)
						{
							pos += condition ? (int)target : 1;
						}
					}
					break;
				case jmp:
					result = stack.pop(&target);
					if(result == Ok) pos += (int)target;
					break;
				case ret:
					pos = -1;
					break;
					
				case get:
				case put:
					result = stack.pop(&target);
					if(result == Ok)
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
							if(result == Ok) memory[addr] = target;
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
						throw "Unknown instruction!";
					}
			}
			if(instruction != jif && instruction != jmp && instruction != ret) ++pos;
		}
		
		if(debugMode & DumpStackAfterEachInstruction)
		{
			if(result == Ok)
			{
				stack.dump(translateInstruction(instruction));
			}
			else
			{
				std::cout << translateInstruction(instruction) << "\t" << translateResult(result) << std::endl;
			}				
		}
		
		++opCount;
	}
	while(result == Ok && pos >= 0 && pos < (int)program.size() && opCount < maxOps);

	if(debugMode & DumpStackOnExit) stack.dump("(exit)");
	
	if(result == Ok && opCount < maxOps)
	{
		// program terminated normally - try and get result
		// peek instruction not part of program so return different error
		if(stack.peek(output) != Ok) result = NoOutput;
	}
	else
	{
		// failed
		if(result == Ok) result = ExceededMaxOpCount;
	}
	return result == Ok;
}

std::string PileUp::getLastError()
{
	return translateResult(result);
}

PileUp::~PileUp()
{
	delete memory;
}