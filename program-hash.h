#pragma once

#include "types.h"

#include <boost/functional/hash.hpp>

// c++ doesn't provide a hash function for std::vector of anything except bool
// this declaration adds a specialization of the hash function for Program
// note: this must be defined before the declaration of unordered_set
namespace std
{
	template <>
	struct hash<Program>
	{
		size_t operator()(const Program& program) const
		{
			return boost::hash_range(program.begin(), program.end());
		}
	};
}