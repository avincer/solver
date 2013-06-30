// Part of Inferer by Andrew Vincer & Alan Tennant.
// alan2here@gmail.com

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // for rand and abs
#include <ctime>
#include "ivm.h"

namespace VMs
{
    class IInst
    {
        unsigned short ways_to_expand();

        void expand(unsigned short n);
    };

    namespace newtAble
    {
        class instructions : public IInst
        { // needs an interface, put at the top of this file
            public:
                friend class VM;

                instructions() {
                    ways = 11;}

                unsigned short ways_to_expand() {
                    return ways;}

                void expand(unsigned short n)
                {
                    // 11 instructions
                    store.insert(store.begin() + (n % 11), n);
                    ways = (store.size() + 1) * 11;
                }

            private:
                std::vector<unsigned short> store;
                unsigned short ways;
        };

        class VM : public IVM
        {
            public:
                unsigned int
                    most_run_time_per_output,
                    largest_code_runTime_size;

                VM() {
                    code_pos = 0;
                    latest_output_p = nullptr;
                    most_run_time_per_output = 100;
                    largest_code_runTime_size = 40;
                    debug_info = false;}

                void set_code(
                    instructions code) {
                    this->code = code;
                    reset();}

                void reset() {
                    if (latest_output_p != nullptr) {
                        delete latest_output_p;
                        latest_output_p = nullptr;}
                    code_pos = 0;
                    first_value = true;
                    can_run_again_p = code.store.size() != 0;}

                bool can_run_again() {
                    return can_run_again_p;}

                unsigned short* latest_output() {
                    return latest_output_p;}

                void run()
                {
                    bool cont = can_run_again_p;
                    if (first_value)
                        code_runTime_copy = code;
                    size_t code_runTime_size =
                        code_runTime_copy.store.size();
                    unsigned short
                        instruction,
                        run_time_this_output = 0;
                    const unsigned short
                        instructions = 11;
                    while(cont)
                    {
                        instruction = code_runTime_copy.store[code_pos] % instructions;
                        if(debug_info)
                            std::cout<<code_pos<<", ";
                        code_pos = (code_pos + 1) % code_runTime_size;
                        switch(instruction)
                        {
                            case 0:
                                if(debug_info)
                                    std::cout<<"end";
                                cont = false;
                                can_run_again_p = false;
                                if (latest_output_p != nullptr)
                                {
                                    delete latest_output_p;
                                    latest_output_p = nullptr;}
                            break;
                            case 1:
                                if(debug_info)
                                    std::cout<<"goto p1";
                                code_pos = code_runTime_copy.store[
                                    code_pos % code_runTime_size];
                            break;
                            case 2:
                                if(debug_info)
                                    std::cout<<"if value at p1 % 2 = 0 then goto p2";
                                if(code_runTime_copy.store[
                                    code_runTime_copy.store[code_pos]
                                        % code_runTime_size] % 2 == 0)
                                    code_pos = code_runTime_copy.store[
                                        (code_pos + 1) % code_runTime_size];
                                else
                                    code_pos += 2;
                            break;
                            case 3:
                                if(debug_info)
                                    std::cout<<"value at p1 = value at p2";
                                code_runTime_copy.store[
                                    code_runTime_copy.store[code_pos]
                                    % code_runTime_size] =
                                        code_runTime_copy.store[
                                            code_runTime_copy.store[
                                                (code_pos + 1) % code_runTime_size]
                                            % code_runTime_size];
                                code_pos += 2;
                            break;
                            case 4:
                                if(debug_info)
                                    std::cout<<"value at p1 = value at p2 + value at p3";
                                code_runTime_copy.store[
                                    code_runTime_copy.store[code_pos] % code_runTime_size] = (
                                        code_runTime_copy.store[
                                            code_runTime_copy.store[
                                            (code_pos + 1) % code_runTime_size]
                                            % code_runTime_size] +
                                        code_runTime_copy.store[
                                            code_runTime_copy.store[
                                                (code_pos + 2) % code_runTime_size] % code_runTime_size]
                                        ) % USHRT_MAX;
                                code_pos += 3;
                            break;
                            case 5:
                            {
                                if(debug_info)
                                    std::cout<<"value at p1 = value at p2 - value at p3";
                                long v1 =
                                (long)code_runTime_copy.store[
                                    code_runTime_copy.store[
                                        (code_pos + 1) % code_runTime_size]
                                    % code_runTime_size] -
                                    code_runTime_copy.store[
                                        code_runTime_copy.store[
                                            (code_pos + 2) % code_runTime_size]
                                        % code_runTime_size];
                                code_runTime_copy.store[
                                    code_runTime_copy.store[code_pos]
                                        % code_runTime_size] = abs(v1) % USHRT_MAX;
                                code_pos += 3;
                            }
                            break;
                            case 6:
                            {
                                if(debug_info)
                                    std::cout<<"toggle value at p1";
                                size_t v1 = code_runTime_copy.store[code_pos]
                                    % code_runTime_size;
                                unsigned short v2 = code_runTime_copy.store[v1];
                                if(v2 == 0) {code_runTime_copy.store[v1] = 1;}
                                else {code_runTime_copy.store[v1] = 0;}
                                code_pos++;
                            }
                            break;
                            case 7:
                                if(debug_info)
                                    std::cout<<"output value at p1";
                                if (latest_output_p != nullptr)
                                    delete latest_output_p;
                                latest_output_p = new unsigned short(
                                    code_runTime_copy.store[
                                        code_runTime_copy.store[code_pos]
                                        % code_runTime_size]);
                                code_pos++;
                                cont = false;
                            break;
                            case 8:
                                if(debug_info)
                                    std::cout<<"increase size";
                                code_runTime_copy.store.push_back(0);
                                if (code_pos == 0)
                                    code_pos = code_runTime_size;
                            break;
                            case 9:
                            {
                                if(debug_info)
                                    std::cout<<"increment value at p1";
                                size_t v1 = code_runTime_copy.store[code_pos]
                                    % code_runTime_size;
                                code_runTime_copy.store[v1] =
                                    (code_runTime_copy.store[v1] + 1) % USHRT_MAX;
                                code_pos++;
                            }
                            break;
                            case 10:
                            {
                                if(debug_info)
                                    std::cout<<"decrement value at p1";
                                size_t v1 = code_runTime_copy.store[code_pos]
                                    % code_runTime_size;
                                code_runTime_copy.store[v1] =
                                    abs((code_runTime_copy.store[v1] - 1)
                                    % USHRT_MAX);
                                code_pos++;
                            }
                            break;
                        }
                        if(debug_info)
                            std::cout<<std::endl;
                        run_time_this_output++;
                        code_runTime_size = code_runTime_copy.store.size();
                        code_pos = code_pos % code_runTime_size;
                        if(run_time_this_output == most_run_time_per_output) {
                            cont = false;
                            can_run_again_p = false;}
                        if(code_runTime_size > largest_code_runTime_size)
                        {
                            cont = false;
                            can_run_again_p = false;
                            if (debug_info)
                                std::cout<<"became too large"<<std::endl;
                        }
                    }
                    first_value = false;
                }

            private:
                VMs::newtAble::instructions
                    code,
                    code_runTime_copy;
                bool can_run_again_p,
                    debug_info,
                    first_value;
                unsigned short* latest_output_p;
                size_t code_pos;
        };
    }
}
