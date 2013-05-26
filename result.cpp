#include "result.h"

const char* translateResult(Result result)
{
	switch(result)
	{
		case Ok:
			return "ok";
		case StackUnderflow:
			return "stack underflow";
		case StackOverflow:
			return "stack overflow";
		case ExceededMaxOpCount:
			return "exceeded max op count";
		case NoOutput:
			return "no output";
		case MathError:
			return "math error";
		default:
			return "???";
	}
}