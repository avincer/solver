typedef enum
{
	Ok,
	StackUnderflow,
	StackOverflow,
	ExceededMaxOpCount,
	NoOutput
}
Result;

const char* translateResult(Result result);