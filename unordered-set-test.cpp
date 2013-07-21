#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <iostream>
#include <map>
#include <set>

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
	const size_t programCount = 1000000;
	const unsigned char instructionCount = 60;
	
	std::unordered_set<Program> programs;
	programs.max_load_factor(1);
	
	std::cout << "Storing " << programCount << " random programs..." << std::endl;
	
	std::set<size_t> rawHashes;
	size_t rawCollisions = 0;
	
	Program program;
	for(size_t i = 0; i < programCount; ++i)
	{
		program.clear();
		do
		{
			program.push_back(rand() % instructionCount);
		}
		while(programs.find(program) != programs.end());
		programs.emplace(program);
		
		auto hash = hashProgram(program);
		if(rawHashes.find(hash) == rawHashes.end())
		{
			rawHashes.emplace(hash);
		}
		else
		{
			++rawCollisions;
		}
	}
	
	auto bucketCount = programs.bucket_count();
	std::cout << "Stored " << programs.size() << " programs in " << bucketCount << " buckets" << std::endl;
	std::cout << "Load factor: " << programs.load_factor() << " / " << programs.max_load_factor() << std::endl;
	std::cout << std::endl;
	
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
	
	return 0;
}