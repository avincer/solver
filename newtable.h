#define use_D

#ifdef use_A
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

			void loadProgram(const Program& program);

			std::string returnStringForm(const Program& program);

			std::string last_runTime_code();

			void reset();

            std::string getLastError();

            bool runTime_code_output;

			bool run(float seed, float* output);

		private:
			int maxOps;
            Program
                code,
                code_runTime_copy;
            std::vector<float> code_floats;
            bool first_run_this_program;
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
#endif

#ifdef use_B
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

            void loadProgram(const Program& program);

            std::string returnStringForm(const Program& program);

            std::string last_runTime_code();

            void reset();

            std::string getLastError();

            bool runTime_code_output;

            bool run(float seed, float* output);

        private:
            int maxOps;
            Program
                code;
            std::vector<float> code_floats;
            bool first_run_this_program;
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
#endif

#ifdef use_C
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

            void loadProgram(const Program& program);

            std::string returnStringForm(const Program& program);

            std::string last_runTime_code();

            void reset();

            std::string getLastError();

            bool runTime_code_output;

            bool run(float seed, float* output);

        private:
            int maxOps;
            Program code;
            std::vector<float> code_runTime_copy;
            bool first_run_this_program;
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
#endif

#ifdef use_D
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

            void loadProgram(const Program& program);

            std::string returnStringForm(const Program& program);

            void reset();

            std::string getLastError();

            bool run(float seed, float* output);

        private:
            int maxOps;
            Program code;
            std::vector<float> floats;
            size_t floats_size, code_size;
            bool first_run_this_program;
            std::string last_error;
            unsigned int
                most_run_time_per_output,
                largest_code_runTime_size;

            const static std::vector<std::string> instruction_names;
            const static std::vector<unsigned char> instruction_params;

            size_t vec_safe(float at, size_t max);
            float lookup(unsigned short pos);
    };
}
#endif
