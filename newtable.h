#include <vector>
#include <string>
#include "ivm.h"

namespace newtAble
{
	class VM : public IVM
	{
		public:
			VM(unsigned int most_run_time_per_output);

			std::string getName();

			int supportedInstructionCount();

			void loadProgram(const std::vector<int>& program);

			std::string returnStringForm(const std::vector<int>& program);

			std::string last_runTime_code();

			void reset();

			std::string getLastError();

			bool runTime_code_output;

			bool run(float seed, float* output);

		private:
			int maxOps;
			std::vector<int>
				code,
				code_runTime_copy;
			bool first_value;
			size_t code_pos;
			std::string
				last_error,
				last_runTime_code_p;
			unsigned int
				most_run_time_per_output,
				largest_code_runTime_size;
			const static std::vector<std::string> instruction_names;
	};
}