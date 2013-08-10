#include <vector>
#include <cstddef>

#include "types.h"

typedef enum
{
	addition,
	removal,
	substitution
}
MutationType;

typedef size_t Mutation;

// note: these are here to support unit tests
#define createMutation(_type, _instruction, _position) (_type | (_instruction << 8) | (_position << 16))
#define getMutationType(_mutation) (_mutation & 0xff)
#define getMutationInstruction(_mutation) ((_mutation >> 8) & 0xff)
#define getMutationPosition(_mutation) (_mutation >> 16)

class MutationCache
{
	private:
		const unsigned char instructionCount;
		
		// map from program length to list of all mutations
		std::vector<std::vector<Mutation>> mutationCache;
	
	public:
		MutationCache(unsigned char instructionCount);
		
		const std::vector<Mutation>& getMutations(size_t programLen);
		
		Program mutateProgram(const Program& program, Mutation mutation);
};