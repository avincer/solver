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

int main()
{
	const size_t programCount = 10000000;
	const unsigned char instructionCount = 38;
	const bool useCache = true;
	
	std::unordered_set<Program> programs;
	programs.max_load_factor(1);
	programs.reserve(programCount);
	
	std::cout << "Storing " << programCount << " random programs..." << std::endl;
	
	//std::set<size_t> rawHashes;
	//size_t rawCollisions = 0;
	
	size_t maxLen = floor(sizeof(size_t) * 8 * log(2) / log(instructionCount));
	std::vector<size_t> remaining; // program length -> programs of that length yet to be run 
	remaining.resize(maxLen + 1);
	
	size_t permutations = 1;
	for(auto i = 1; i <= maxLen; ++i) {
		remaining[i] = (permutations *= instructionCount);
		std::cout << remaining[i] << " programs of length " << i << std::endl;
	}
	
	size_t cacheQueries = 0, setQueries = 0;
	
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
			
			if(useCache && len <= maxLen && !remaining[len])
			{
				// no programs remaining of this length, cannot create one
				++cacheQueries;
			}
			else
			{
				// program longer than cache or still some remaining - check
				++setQueries;
				createdProgram = programs.insert(program).second;
				if(createdProgram) --remaining[len];
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
		std::cout << remaining[i] << " programs remaining of length " << i << std::endl;
	}
	
	auto bucketCount = programs.bucket_count();
	std::cout << std::endl;
	std::cout << "Stored " << programs.size() << " programs in " << bucketCount << " buckets" << std::endl;
	std::cout << "Load factor: " << programs.load_factor() << " / " << programs.max_load_factor() << std::endl;
	std::cout << "Cache queries: " << cacheQueries << std::endl;
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