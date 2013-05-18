typedef enum
{
	// push //
	// note: these must be contiguous and in order!
	pushNeg1,
	push0,
	push1,
	push2,
	push3,
	push4,
	push5,
	push6,
	push7,
	push8,
	push9,
	push10,
	
	// stack //
	dup,
	swap,
	drop,
	
	// unary math //
	neg,
	inc,
	dec,
	truncate,
	
	// unary logic //
	lnot,
	
	// binary math //
	add,
	sub,
	mul,
	divide,
	fmodulus,
	
	// binary logic //
	land,
	lor,
	
	// comparators //
	eq,
	neq,
	gt,
	lt,
	gte,
	lte,
	
	// flow control
	jif,
	jmp,
	ret,
	
	// memory
	get,
	put,
	
	InstructionCount // must be last!
}
Instruction;