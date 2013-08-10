#include <cstdlib> // for rand and abs
#include <ctime>
#include <climits>
#include "newtable.h"

namespace newtAble
{
	VM::VM(unsigned int most_run_time_per_output) {
		code_pos = 0;
		this->most_run_time_per_output = most_run_time_per_output;
		largest_code_runTime_size = 40;
		runTime_code_output = false;}

	std::string VM::getName() {
		return "newtAble";}

	int VM::supportedInstructionCount() {
		return 11;}

	void VM::loadProgram(
		const Program& program) {
		code = program;
		reset();}

	std::string VM::returnStringForm(
		const Program& program)
	{
		std::string str;
		auto code_length = program.size();
		for (unsigned short n = 0; n < code_length; n++)
			str += instruction_names[program[n]] +
				((n < code_length - 1) ? " ": "");
		// VM tmp();
		// tmp.loadProgram(program);
		// str += ", run time: ";
		// float* output;
		// tmp.runTime_code_output = true;
		// tmp.run(0, output, 100000);
		// str += tmp.last_runTime_code();
		return str;
	}

	std::string VM::last_runTime_code() {
		return last_runTime_code_p;}

	void VM::reset() {
		code_pos = 0;
		first_value = true;}

	std::string VM::getLastError() {
		return last_error;}

	bool VM::run(float seed, float* output)
	{
		bool cont = true,
			ok_run = true;
		last_error = "";
		last_runTime_code_p = "";
		if (first_value)
			code_runTime_copy = code;
		size_t code_runTime_size =
			code_runTime_copy.size();
		unsigned short
			instruction,
			run_time_this_output = 0;
		const unsigned short
			instructions = 11;
		while(cont)
		{
			instruction = code_runTime_copy[
				code_pos] % instructions;
			code_pos = (code_pos + 1) % code_runTime_size;
			switch(instruction)
			{
				case 0:
					if(runTime_code_output)
						last_runTime_code_p += "end";
					cont = false;
					ok_run = false;
					last_error = "program complete";
				break;
				case 1:
					if(runTime_code_output)
						last_runTime_code_p += "goto p1";
					code_pos = code_runTime_copy[
						code_pos % code_runTime_size];
				break;
				case 2:
					if(runTime_code_output)
						last_runTime_code_p += "if @p1 % 2 = 0 then goto p2";
					if(code_runTime_copy[
						code_runTime_copy[code_pos]
							% code_runTime_size] % 2 == 0)
						code_pos = code_runTime_copy[
							(code_pos + 1) % code_runTime_size];
					else
						code_pos += 2;
				break;
				case 3:
					if(runTime_code_output)
						last_runTime_code_p += "@p1 = @p2";
					code_runTime_copy[
						code_runTime_copy[code_pos]
						% code_runTime_size] =
							code_runTime_copy[
								code_runTime_copy[
									(code_pos + 1) % code_runTime_size]
								% code_runTime_size];
					code_pos += 2;
				break;
				case 4:
					if(runTime_code_output)
						last_runTime_code_p += "@p1 = @p2 + @p3";
					code_runTime_copy[
						code_runTime_copy[code_pos] % code_runTime_size] = (
							code_runTime_copy[
								code_runTime_copy[
								(code_pos + 1) % code_runTime_size]
								% code_runTime_size] +
							code_runTime_copy[
								code_runTime_copy[
									(code_pos + 2) % code_runTime_size] % code_runTime_size]
							) % USHRT_MAX;
					code_pos += 3;
				break;
				case 5:
				{
					if(runTime_code_output)
						last_runTime_code_p += "@p1 = @p2 - @p3";
					long v1 =
					(long)code_runTime_copy[
						code_runTime_copy[
							(code_pos + 1) % code_runTime_size]
						% code_runTime_size] -
						code_runTime_copy[
							code_runTime_copy[
								(code_pos + 2) % code_runTime_size]
							% code_runTime_size];
					code_runTime_copy[
						code_runTime_copy[code_pos]
							% code_runTime_size] = abs(v1) % USHRT_MAX;
					code_pos += 3;
				}
				break;
				case 6:
				{
					if(runTime_code_output)
						last_runTime_code_p += "toggle @p1";
					size_t v1 = code_runTime_copy[code_pos]
						% code_runTime_size;
					unsigned short v2 = code_runTime_copy[v1];
					if(v2 == 0) {code_runTime_copy[v1] = 1;}
					else {code_runTime_copy[v1] = 0;}
					code_pos++;
				}
				break;
				case 7:
					if(runTime_code_output)
						last_runTime_code_p += "output @p1";
					*output = code_runTime_copy[
							code_runTime_copy[code_pos]
							% code_runTime_size];
					code_pos++;
					cont = false;
				break;
				case 8:
					if(runTime_code_output)
						last_runTime_code_p += "increase size";
					code_runTime_copy.push_back(0);
					if (code_pos == 0)
						code_pos = code_runTime_size;
				break;
				case 9:
				{
					if(runTime_code_output)
						last_runTime_code_p += "increment @p1";
					size_t v1 = code_runTime_copy[code_pos]
						% code_runTime_size;
					code_runTime_copy[v1] =
						(code_runTime_copy[v1] + 1) % USHRT_MAX;
					code_pos++;
				}
				break;
				case 10:
				{
					if(runTime_code_output)
						last_runTime_code_p += "decrement @p1";
					size_t v1 = code_runTime_copy[code_pos]
						% code_runTime_size;
					code_runTime_copy[v1] =
						abs((code_runTime_copy[v1] - 1)
						% USHRT_MAX);
					code_pos++;
				}
				break;
			}
			run_time_this_output++;
			code_runTime_size = code_runTime_copy.size();
			code_pos = code_pos % code_runTime_size;
			if(run_time_this_output == most_run_time_per_output) {
				cont = false;
				ok_run = false;
				last_error = "out of time";}
			if(code_runTime_size > largest_code_runTime_size) {
				cont = false;
				ok_run = false;
				last_error = "code too large";}
			if(runTime_code_output && cont)
				last_runTime_code_p += " ";
		}
		first_value = false;
		return ok_run;
	}

	const std::vector<std::string> VM::instruction_names =
		{"end", "goto", "if", "=", "+", "-", "toggle",
			"output", "increase", "increment", "decrement"};
}