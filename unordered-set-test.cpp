#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <iostream>
#include <map>
#include <set>
#include <cmath>

typedef std::vector<unsigned char> Program;

size_t hashProgram(const Program& program)
{
	return boost::hash_range(program.begin(), program.end());
}

namespace std
{
	template <>
	struct hash<Program>
	{
		size_t operator()(const Program& program) const
		{
			return hashProgram(program);
		}
	};
}

typedef struct
{
	size_t count;
	size_t remaining;
}
Permutation;

int main()
{
	const size_t programCount = 10000000;
	const unsigned char instructionCount = 38;
	const bool useCache = true;
	const bool useGuess = true;
	
	std::unordered_set<Program> programs;
	programs.max_load_factor(1);
	programs.reserve(programCount);
	
	std::cout << "Storing " << programCount << " random programs..." << std::endl;
	
	//std::set<size_t> rawHashes;
	//size_t rawCollisions = 0;
	
	size_t maxLen = floor(sizeof(size_t) * 8 * log(2) / log(instructionCount));
	std::vector<Permutation> permutations;
	permutations.resize(maxLen + 1);
	
	size_t count = 1;
	for(auto i = 1; i <= maxLen; ++i) {
		count *= instructionCount;
		permutations[i].count = permutations[i].remaining = count;
	}
	
	size_t cacheQueries = 0, guesses = 0, setQueries = 0;
	
	Program program;
	for(size_t i = 0; i < programCount; ++i)
	{
		program.clear();
		size_t len = 0;
		auto createdProgram = false;
		do
		{
			program.push_back(rand() % instructionCount);
			++len;
			
			auto querySet = true;
			
			if(useCache && len <= maxLen)
			{
				// if there are no programs remaining of this length we cannot create one
				if(!permutations[len].remaining)
				{
					++cacheQueries;
					querySet = false;
				}
				else if(useGuess)
				{
					// if there are 10% of programs of this length remaining, check 10% of the time
					// assume program already exists 90% of the time (even though it may not)
					// note: favours generation of slightly longer programs
					auto threshold = (size_t)(permutations[len].remaining * (double)RAND_MAX / permutations[len].count);
					if(threshold < (rand() % RAND_MAX))
					{
						++guesses;
						querySet = false;
					}
				}
			}
			
			if(querySet)
			{
				// program longer than cache or still some remaining - check
				++setQueries;
				createdProgram = programs.insert(program).second;
				if(createdProgram && len <= maxLen) --permutations[len].remaining;
			}
		}
		while(!createdProgram);
		//programs.emplace(program);
		
		/*
		auto hash = hashProgram(program);
		if(rawHashes.find(hash) == rawHashes.end())
		{
			rawHashes.emplace(hash);
		}
		else
		{
			++rawCollisions;
		}
		*/
	}
	
	std::cout << std::endl;
	for(auto i = 1; i <= maxLen; ++i) {
		std::cout << permutations[i].remaining << " / " << permutations[i].count << " programs of length " << i << " remaining";
		std::cout << " (" << (permutations[i].remaining / (double)permutations[i].count) * 100 << "%)" << std::endl;
	}
	
	auto bucketCount = programs.bucket_count();
	std::cout << std::endl;
	std::cout << "Stored " << programs.size() << " programs in " << bucketCount << " buckets" << std::endl;
	std::cout << "Load factor: " << programs.load_factor() << " / " << programs.max_load_factor() << std::endl;
	std::cout << "Cache queries: " << cacheQueries << std::endl;
	std::cout << "Guesses: " << guesses << std::endl;
	std::cout << "Set queries: " << setQueries << std::endl;
	std::cout << std::endl;
	
	/*
	std::map<size_t, size_t> bucketSizes;
	for(auto i = 0; i < bucketCount; ++i)
	{
		bucketSizes[programs.bucket_size(i)]++;
	}
	
	size_t collisions = 0;
	std::cout << "Bucket size: count" << std::endl;
	for(auto size: bucketSizes)
	{
		auto bucketSize = size.first;
		auto count = size.second;
		std::cout << bucketSize << ": " << count << std::endl;
		if(bucketSize > 1) collisions += (bucketSize - 1) * count;
	}
	
	std::cout << std::endl;
	std::cout << "Average non-empty bucket size: " << programCount / (double)(bucketCount - bucketSizes[0]) << std::endl;
	std::cout << "Collisions: " << collisions << std::endl;
	std::cout << "Raw collisions: " << rawCollisions << std::endl;
	*/
	return 0;
}