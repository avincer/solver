#define use_D

#ifdef use_A
#include <cstdlib> // for rand and abs
#include <ctime>
#include <climits>
#include <cmath>
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
        return 14;}

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
        first_run_this_program = true;}

	std::string VM::getLastError() {
		return last_error;}

    bool VM::run(float seed, float* output)
	{
		bool cont = true,
            ok_run = true;
		last_error = "";
		last_runTime_code_p = "";
        if (first_run_this_program)
        {
			code_runTime_copy = code;
            code_floats.clear();
            code_floats.push_back(0.25);
            code_floats.push_back(1);
            code_floats.push_back(3);
        }
        auto code_runTime_size =
            code_runTime_copy.size();
        size_t code_floats_size;
		unsigned short
			instruction,
			run_time_this_output = 0;
		const unsigned short
            instructions = 14;
		while(cont)
		{
			instruction = code_runTime_copy[
				code_pos] % instructions;
			code_pos = (code_pos + 1) % code_runTime_size;
            code_floats_size = code_floats.size();
			switch(instruction)
			{
                // R is runtime code.
                // F is floats.
                // P1, P2 and P3 are param 1, 2 and 3.
				case 0:
					if(runTime_code_output)
						last_runTime_code_p += "end";
					cont = false;
					ok_run = false;
					last_error = "program complete";
				break;
				case 1:
					if(runTime_code_output)
                        last_runTime_code_p += "goto R[p1]";
					code_pos = code_runTime_copy[
						code_pos % code_runTime_size];
				break;
				case 2:
					if(runTime_code_output)
                        last_runTime_code_p += "if R[R[p1]] % 2 = 0 then goto R[p2]";
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
                        last_runTime_code_p += "R[R[p1]] = R[R[p2]]";
					code_runTime_copy[
						code_runTime_copy[code_pos]
						% code_runTime_size] =
                        code_runTime_copy[code_runTime_copy[
                        (code_pos + 1) % code_runTime_size]
                        % code_runTime_size];
					code_pos += 2;
				break;
                case 4:
                    if(runTime_code_output)
                        last_runTime_code_p += "F[R[p1]] = F[R[p2]]";
                    code_floats[
                        code_runTime_copy[code_pos]
                        % code_floats_size] =
                        code_floats[code_runTime_copy[
                        (code_pos + 1) % code_runTime_size]
                        % code_floats_size];
                    code_pos += 2;
                break;
                case 5:
					if(runTime_code_output)
                        last_runTime_code_p += "R[R[p1]] = R[R[p2]] + R[R[p3]]";
					code_runTime_copy[
						code_runTime_copy[code_pos] % code_runTime_size] = (
							code_runTime_copy[
								code_runTime_copy[
								(code_pos + 1) % code_runTime_size]
								% code_runTime_size] +
							code_runTime_copy[
								code_runTime_copy[
									(code_pos + 2) % code_runTime_size] % code_runTime_size]
                            ) % UCHAR_MAX;
					code_pos += 3;
				break;
                case 6:
				{
					if(runTime_code_output)
                        last_runTime_code_p += "R[R[p1]] = R[R[p2]] - R[R[p3]]";
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
                            % code_runTime_size] = abs(v1) % UCHAR_MAX;
					code_pos += 3;
				}
				break;
                case 7:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[R[p1]] = F[R[p2]] + F[R[p3]]";
                    auto t1 = code_floats[
                            code_runTime_copy[
                            (code_pos + 1) % code_runTime_size]
                            % code_floats_size];
                    auto t2 = code_floats[
                            code_runTime_copy[
                            (code_pos + 2) % code_runTime_size] % code_floats_size];
                    if (t1 == -INFINITY || t1 == INFINITY ||
                        t2 == -INFINITY || t2 == INFINITY) {
                        t1 = 0;
                        t2 = 0;}
                    code_floats[
                        code_runTime_copy[code_pos] % code_floats_size] = t1 + t2;
                    code_pos += 3;
                }
                break;
                case 8:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[R[p1]] = F[R[p2]] - F[R[p3]]";
                    auto t1 = code_floats[
                        code_runTime_copy[
                        (code_pos + 1) % code_runTime_size]
                        % code_floats_size];
                    auto t2 = code_floats[
                        code_runTime_copy[
                        (code_pos + 2) % code_runTime_size] % code_floats_size];
                    if (t1 == -INFINITY || t1 == INFINITY ||
                        t2 == -INFINITY || t2 == INFINITY) {
                        t1 = 0;
                        t2 = 0;}
                    code_floats[code_runTime_copy[
                        code_pos] % code_floats_size] = t1 - t2;
                    code_pos += 3;
                }
                break;
                case 9:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[R[p1]] = F[R[p2]] * F[R[p3]]";
                    auto t1 = code_floats[
                        code_runTime_copy[
                        (code_pos + 1) % code_runTime_size]
                        % code_floats_size];
                    auto t2 = code_floats[
                        code_runTime_copy[
                        (code_pos + 2) % code_runTime_size] % code_floats_size];
                    if (t1 == -INFINITY || t1 == INFINITY ||
                        t2 == -INFINITY || t2 == INFINITY) {
                        t1 = 0;
                        t2 = 0;}
                    code_floats[
                        code_runTime_copy[code_pos] % code_floats_size] = t1 * t2;
                    code_pos += 3;
                }
                break;
                case 10:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[R[p1]] = SQRT(F[R[p2]])";
                    auto t = code_floats[code_runTime_copy[
                        (code_pos + 1) % code_runTime_size]
                        % code_floats_size];
                    if (t > 2000000000 || t < -2000000000)
                        t = 0;
                    code_floats[
                        code_runTime_copy[code_pos] % code_floats_size] =
                            sqrt(abs(t));
                    code_pos += 2;
                }
                break;
                case 11:
				{
					if(runTime_code_output)
                        last_runTime_code_p += "toggle R[R[p1]]";
                    auto v1 = code_runTime_copy[code_pos]
						% code_runTime_size;
					unsigned short v2 = code_runTime_copy[v1];
					if(v2 == 0) {code_runTime_copy[v1] = 1;}
					else {code_runTime_copy[v1] = 0;}
					code_pos++;
				}
				break;
                case 12:
					if(runTime_code_output)
                        last_runTime_code_p += "output F[R[p1]]";
                    *output = code_floats[
                        code_runTime_copy[code_pos]
                        % code_floats_size];
					code_pos++;
					cont = false;
				break;
                case 13:
					if(runTime_code_output)
                        last_runTime_code_p += "grow R & F by 1";
					code_runTime_copy.push_back(0);
                    code_floats.push_back(0);
					if (code_pos == 0)
                        code_pos = code_runTime_size;
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
        first_run_this_program = false;
		return ok_run;
	}

	const std::vector<std::string> VM::instruction_names =
    {"end", "goto", "if", "R=", "F=", "R+", "R-", "F+", "F-", "F*", "SQRT(F)", "toggle",
            "output", "increase_both"};
}
#endif

#ifdef use_B
#include <cstdlib> // for rand and abs
#include <ctime>
#include <climits>
#include <cmath>
#include "newtable.h"

namespace newtAble
{
    VM::VM(unsigned int most_run_time_per_output) {
        code_pos = 0;
        this->most_run_time_per_output = most_run_time_per_output;
        largest_code_runTime_size = 40;
        runTime_code_output = false;}

    std::string VM::getName() {
        return "newtAble static";}

    int VM::supportedInstructionCount() {
        return 10;}

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
        first_run_this_program = true;}

    std::string VM::getLastError() {
        return last_error;}

    bool VM::run(float seed, float* output)
    {
        bool cont = true,
            ok_run = true;
        last_error = "";
        last_runTime_code_p = "";
        if (first_run_this_program)
        {
            code_floats.clear();
            code_floats.push_back(0.25);
            code_floats.push_back(1);
            code_floats.push_back(3);
        }
        auto code_size = code.size();
        size_t code_floats_size;
        unsigned short
            instruction,
            run_time_this_output = 0;
        const unsigned short
            instructions = 10;
        while(cont)
        {
            instruction = code[code_pos] % instructions;
            code_pos = (code_pos + 1) % code_size;
            code_floats_size = code_floats.size();
            switch(instruction)
            {
                // C is code.
                // F is floats.
                // P1, P2 and P3 are param 1, 2 and 3.
                case 0:
                    if(runTime_code_output)
                        last_runTime_code_p += "end";
                    cont = false;
                    ok_run = false;
                    last_error = "program complete";
                break;
                case 1:
                    if(runTime_code_output)
                        last_runTime_code_p += "goto C[p1]";
                    code_pos = code[code_pos % code_size];
                break;
                case 2:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "if F[F[p1]] < 1 then goto C[p2]";
                    auto t = code_floats[code_pos];
                    if (t < 9000000000000000000 || t > 9000000000000000000)
                        t = 0;
                    if(code_floats[((int)t) % code_floats_size] < 1)
                        code_pos = code[(code_pos + 1) % code_size];
                    else
                        code_pos += 2;
                }
                break;
                case 3:
                    if(runTime_code_output)
                        last_runTime_code_p += "F[C[p1]] = F[C[p2]]";
                    code_floats[
                        code[code_pos] % code_floats_size] =
                        code_floats[code[(code_pos + 1) % code_size]
                        % code_floats_size];
                    code_pos += 2;
                break;
                case 4:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[F[C[p1]]] = F[F[C[p2]]] + F[F[C[p3]]]";
                    auto t1 = code_floats[((int)code_floats[
                            code[(code_pos + 1) % code_size]
                            % code_floats_size]) % code_floats_size];
                    auto t2 = code_floats[((int)code_floats[
                            code[(code_pos + 2) % code_size] % code_floats_size]) % code_floats_size];
                    if (t1 == -INFINITY || t1 == INFINITY ||
                        t2 == -INFINITY || t2 == INFINITY) {
                        t1 = 0;
                        t2 = 0;}
                    code_floats[((int)code_floats[
                        code[code_pos] % code_floats_size]) % code_floats_size] = t1 + t2;
                    code_pos += 3;
                }
                break;
                case 5:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[F[C[p1]]] = F[F[C[p2]]] - F[F[C[p3]]]";
                    auto t1 = code_floats[((int)code_floats[
                        code[(code_pos + 1) % code_size]
                        % code_floats_size]) % code_floats_size];
                    auto t2 = code_floats[((int)code_floats[
                        code[(code_pos + 2) % code_size] % code_floats_size]) % code_floats_size];
                    if (t1 == -INFINITY || t1 == INFINITY ||
                        t2 == -INFINITY || t2 == INFINITY) {
                        t1 = 0;
                        t2 = 0;}
                    code_floats[((int)code_floats[
                        code[code_pos] % code_floats_size]) % code_floats_size] = t1 - t2;
                    code_pos += 3;
                }
                break;
                case 6:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[C[p1]] = F[C[p2]] * F[C[p3]]";
                    auto t1 = code_floats[
                        code[(code_pos + 1) % code_size]
                        % code_floats_size];
                    auto t2 = code_floats[
                        code[(code_pos + 2) % code_size] % code_floats_size];
                    if (t1 == -INFINITY || t1 == INFINITY ||
                        t2 == -INFINITY || t2 == INFINITY) {
                        t1 = 0;
                        t2 = 0;}
                    code_floats[
                        code[code_pos] % code_floats_size] = t1 * t2;
                    code_pos += 3;
                }
                break;
                case 7:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "F[C[p1]] = SQRT(F[C[p2]])";
                    auto t = code_floats[code[(code_pos + 1) % code_size]
                        % code_floats_size];
                    if (t > 2000000000 || t < -2000000000)
                        t = 0;
                    code_floats[code[code_pos] % code_floats_size] = sqrt(abs(t));
                    code_pos += 2;
                }
                break;
                case 8:
                    if(runTime_code_output)
                        last_runTime_code_p += "output F[C[p1]]";
                    *output = code_floats[
                        code[code_pos] % code_floats_size];
                    code_pos++;
                    cont = false;
                break;
                case 9:
                    if(runTime_code_output)
                        last_runTime_code_p += "grow by 1";
                    code_floats.push_back(0);
                    if (code_pos == 0)
                        code_pos = code_size;
                break;
            }
            run_time_this_output++;
            code_size = code.size();
            code_pos = code_pos % code_size;
            if(run_time_this_output == most_run_time_per_output) {
                cont = false;
                ok_run = false;
                last_error = "out of time";}
            if(runTime_code_output && cont)
                last_runTime_code_p += " ";
        }
        first_run_this_program = false;
        return ok_run;
    }

    const std::vector<std::string> VM::instruction_names =
    {"end", "goto", "if", "=", "+", "-", "*", "SQRT", "output", "increase"};
}
#endif

#ifdef use_C
#include <cstdlib> // for rand and abs
#include <ctime>
#include <climits>
#include <cmath>
#include "newtable.h"

namespace newtAble
{
    VM::VM(unsigned int most_run_time_per_output) {
        code_pos = 0;
        this->most_run_time_per_output = most_run_time_per_output;
        largest_code_runTime_size = 40;
        runTime_code_output = false;}

    std::string VM::getName() {
        return "newtAble slim";}

    int VM::supportedInstructionCount() {
        return 4;}

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
        first_run_this_program = true;}

    std::string VM::getLastError() {
        return last_error;}

    bool VM::run(float seed, float* output)
    {
        auto cont = true,
            ok_run = true;
        last_error = "";
        last_runTime_code_p = "";
        int code_runTime_size = code.size();
        code_pos = 0;
        if (first_run_this_program) {
            code_runTime_copy.clear();
            for (int n = 0; n < code_runTime_size; n++)
                code_runTime_copy.push_back(code[n]);}
        unsigned short
            instruction,
            run_time_this_output = 0;
        const unsigned short
            instructions = 4;
        auto fmd = [](float a, float b){
            if (a < -1000000000 || a > 1000000000 ||
                b < -1000000000 || b > 1000000000) {
                a = 1;
                b = 1;}
            return fmod(a, b);};
        auto ab = [](float a){
            return a > 0 ? a : -a;};
        // RTC is runTime_copy
        auto code_RTC_F_get = [](float pos, VM* t){
            return t->code_runTime_copy[(int)(pos > 0 ? pos : -pos) %
                t->code_runTime_copy.size()];};
        auto code_RTC_F_set = [](float pos, float value, VM* t){
            t->code_runTime_copy[(int)(pos > 0 ? pos : -pos) %
                t->code_runTime_copy.size()] = value;};
        while(cont)
        {
            instruction = fmd(code_runTime_copy[code_pos], instructions);
            switch(instruction)
            {
                // R is runtime code.
                // Px are params.
                case 0:
                {
                    if(runTime_code_output)
                        last_runTime_code_p += "if R[R[p1]] % 2 < 1 then goto R[R[p2]]";
                    code_pos++;
                    if(fmd(code_RTC_F_get(code_pos, this), 2) < 1)
                        code_pos = code_RTC_F_get(code_RTC_F_get(code_pos + 1, this), this);
                    else
                        code_pos += 2;
                }
                break;
                case 1:
                    if(runTime_code_output)
                        last_runTime_code_p += "R[R[p1]] = R[R[p2]] + R[R[p3]] - R[R[p4]]";
                    code_pos++;
                    code_RTC_F_set(code_runTime_copy[code_pos],
                        code_RTC_F_get(code_pos + 1, this) +
                        code_RTC_F_get(code_pos + 2, this) -
                        code_RTC_F_get(code_pos + 3, this), this);
                    code_pos += 4;
                break;
                case 2:
                    if(runTime_code_output)
                        last_runTime_code_p += "output R[R[p1]]";
                    *output = code_RTC_F_get(
                        code_RTC_F_get(code_pos + 1, this), this);
                    cont = false;
                break;
                case 3:
                    if(runTime_code_output)
                        last_runTime_code_p += "grow by 1";
                    code_pos++;
                    code_runTime_copy.push_back(0);
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
        first_run_this_program = false;
        return ok_run;
    }

    const std::vector<std::string> VM::instruction_names =
    {"if", "+-", "output", "grow"};
}
#endif

#ifdef use_D
#include <cstdlib> // for rand and abs
#include <ctime>
#include <climits>
#include <cmath>
#include "newtable.h"

namespace newtAble
{
    VM::VM(unsigned int most_run_time_per_output) {
        this->most_run_time_per_output = most_run_time_per_output;
        largest_code_runTime_size = 40;}

    std::string VM::getName() {
        return "newtAble lock";}

    int VM::supportedInstructionCount() {
        return 4;}

    void VM::loadProgram(
        const Program& program) {
        code = program;
        reset();}

    std::string VM::returnStringForm(
        const Program& program)
    {
        std::string str;
        auto code_length = program.size();
        unsigned short n = 0;
        while (n < code_length)
        {
            str += instruction_names[program[n]] +
                ((n < code_length - 1) ? " ": "");
            n += 1 + instruction_params[program[n]];
        }
        return str;
    }

    void VM::reset() {
        first_run_this_program = true;
        floats.clear();
        floats.push_back(1);
        floats.push_back(1);
        floats_size = 2;
        code_size = code.size();}

    std::string VM::getLastError() {
        return last_error;}

    size_t VM::vec_safe(float at, size_t max) {
        auto b = at > 0 ? at : -at;
        return b < 1000000000 ? (int)b % max : 0;}

    float VM::lookup(unsigned short pos)
    {
        return floats[vec_safe(floats[code[pos %
            code_size] % floats_size], floats_size)];
    }

    bool VM::run(float seed, float* output)
    {
        //if (code.size() == 2)
        //{
            //code[0] = 3;
            //code[1] = 2;
        //}
        auto cont = true,
            ok_run = true;
        last_error = "";
        unsigned short
            code_pos = 0,
            instruction,
            run_time_this_output = 0;
        const unsigned short
            instructions = 4;
        auto fmd = [](float a, float b){
            if (a < -1000000000000 || a > 1000000000000 ||
                b < -1000000000000 || b > 1000000000000) {
                a = 1;
                b = 1;}
            return fmod(a, b);};
        auto u_short_safe = [](float a){
            return (a > USHRT_MAX || a < 0) ? 0 : a;};
        auto arith_safe = [](float a){
            return (a < -1000000000 || a > 1000000000)
                ? 0 : a;};
        //auto floats_get_I = [](size_t pos, VM* t){
            //return t->floats[pos % t->floats_size];};
        //auto floats_get_F = [](float pos, VM* t){
            //return t->floats[(int)(pos > 0 ? pos : -pos) %
                //t->floats_size];};
        auto floats_set_F = [](float pos, float value, VM* t){
            t->floats[t->vec_safe(pos,
                t->floats_size)] = value;};
        //auto code_get_I = [](size_t pos, VM* t){
            //return t->code[pos % t->code_size];};
        //auto code_set_I = [](size_t pos, int value, VM* t){
            //t->code[pos % t->code_size] = value;};
        auto lookup2 = [](unsigned short pos, VM* t){
            return t->lookup(pos) +
                (t->lookup(pos + 1) * 4) +
                (t->lookup(pos + 2) * 16);};
        while(cont)
        {
            instruction = fmd(code[code_pos % code_size], instructions);
            code_pos = (code_pos + 1) % code_size;
            switch(instruction)
            {
                // R is runtime code.
                // Px are params.
                case 0:
                    if(fmd(lookup(code_pos), 2) < 1)
                        code_pos = u_short_safe(lookup2(code_pos + 1, this));
                    else
                        code_pos += 4;
                break;
                case 1:
                {
                    auto a = lookup2(code_pos + 3, this);
                    auto b = lookup2(code_pos + 6, this);
                    auto c = lookup2(code_pos + 9, this);
                    floats_set_F(lookup2(code_pos, this),
                        arith_safe(a) + arith_safe(b) -
                        arith_safe(c), this);
                    code_pos += 12;
                }
                break;
                case 2:
                    *output = lookup2(code_pos, this);
                    cont = false;
                break;
                case 3:
                    floats.push_back(0);
                    floats_size += 1;
                break;
            }
            run_time_this_output++;
            if(run_time_this_output == most_run_time_per_output) {
                cont = false;
                ok_run = false;
                last_error = "out of time";}
            if(floats_size > 300) {
                cont = false;
                ok_run = false;
                last_error = "code too large";}
        }
        first_run_this_program = false;
        return ok_run;
    }

    const std::vector<std::string> VM::instruction_names =
    {"if", "=+-", "output", "grow"};
    const std::vector<unsigned char> VM::instruction_params =
    {4, 12, 3, 0};
}
#endif
