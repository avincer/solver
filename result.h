typedef enum
{
	Ok,
	StackUnderflow,
	StackOverflow,
	ExceededMaxOpCount,
	NoOutput,
	MathError,
}
Result;

const char* translateResult(Result result);