typedef enum
{
	Ok = 1,
	StackUnderflow = -1,
	StackOverflow = -2,
	ExceededMaxOpCount = -3,
	NoOutput = -4
}
Result;

const char* translateResult(Result result);