#include <vector>
#include <unordered_set>

class IProgramDB
{
	public:
		virtual void storeProgram(const std::vector<int>& program) = 0;

		virtual bool findProgram(const std::vector<int>& program) = 0;
};

class SetProgramDB : public IProgramDB
{
	private:
		std::unordered_set<std::vector<int>> programs;

	public:
		void storeProgram(const std::vector<int>& program);

		bool findProgram(const std::vector<int>& program);
};