#include "types.h"

#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>

class IProgramDB
{
	public:
		virtual void storeProgram(const Program& program) = 0;

		virtual bool findProgram(const Program& program) = 0;
};

// c++ doesn't provide a hash function for std::vector of anything except bool
// this declaration adds a specialization of the hash function for Program
namespace std
{
	template <>
	struct hash<Program>
	{
		size_t operator()(const Program & t) const
		{
			return boost::hash_range(t.begin(), t.end());
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