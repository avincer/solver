#include "types.h"

#include <vector>
#include <unordered_set>

class IProgramDB
{
	public:
		virtual void storeProgram(const Program& program) = 0;

		virtual bool findProgram(const Program& program) = 0;
};

// c++ doesn't provide a hash function for std::vector of anything, so we implement one here
// _Hash_seq is an FNV-1a hash function used internally for other types, including strings
namespace std
{
	template <>
	struct hash<Program>
	{
		size_t operator()(const Program & t) const
		{
			return _Hash_seq(t.data(), t.size());
		}
	};
}

class SetProgramDB : public IProgramDB
{
	private:
		std::unordered_set<Program> programs;

	public:
		void storeProgram(const Program& program);

		bool findProgram(const Program& program);
};