#include "solver.h"
#include "pile-up.h"
#include "tree.h"

#include <csignal>
#include <memory>
#include <iostream>
#include <sstream>

// os specific headers
// note: can dump preprocessor macros for gcc using gcc -dM -E - < /dev/null
// gcc defines linux, __linux and __linux__
// s/o suggests using __linux
// http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#ifdef __linux
#include <fenv.h>
#endif

std::unique_ptr<Solver> solver;

void pause(int signal);

int main()
{
	// todo - find windows equivalent of this
#ifdef __linux
	// throw on floating point exceptions
	feenableexcept(FE_INVALID | FE_OVERFLOW);
#endif
	
	// todo - allow selection of the VM (and options)
	int stackSize = 16, memorySize = 16, maxOps = 1000;
	std::unique_ptr<IVM> vm(new PileUp::VM(stackSize, memorySize, maxOps));
	
	// todo - allow selection of random seed?
	auto seed = (int)time(nullptr);
	
	std::unique_ptr<IRandom> random(new CStdRandom());
	random->init(seed);
	
	// todo - allow setting initial weights
	std::vector<double> initialWeights(vm->getInstructionCount(), 0.5);
	
	// todo - allow selecting which program factory to use
	std::unique_ptr<IProgramFactory> factory(new ProgramTree(random.get(), initialWeights, Directed));
	
	// 3x + 1
	std::vector<float> lin { 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31 };

	std::vector<float> alt { 0, 1, -2, 3, -4, 5, -6, 7, -8, 9 };
	
	// x^2 + x - 3
	std::vector<float> quad1 { -3, -1, 3, 9, 17, 27, 39, 53, 69, 87, 107 };
	
	// x^2 + x - 6 (x in [-5, 4])
	std::vector<float> quad2 { 14, 6, 0, -4, -6, -6, -4, 0, 6, 14 };
	
	// todo - get target sequence from somewhere!
	std::vector<float> target = quad1;
	
	// terminate on user signal
	signal(SIGINT, pause);
	
	// build and run the solver (go put the kettle on...)
	solver.reset(new Solver(factory.get(), vm.get(), target));
	solver->run();
	
	return 0;
}

void pause(int signal)
{
	std::cout << std::endl;
	std::cout << "Paused." << std::endl;
	std::cout << std::endl;
	std::cout << "c - continue" << std::endl;
	std::cout << "i /path/to/program - show program information" << std::endl;
	std::cout << "s - save program tree to xml" << std::endl;
	std::cout << "q - quit" << std::endl;
	std::cout << std::endl;
	
	while(1)
	{
		std::cout << "> ";
		char cmd;
		std::cin >> cmd;

		// note use of braces to prevent "jump to case label crosses initialization" error
		switch(cmd)
		{
			case 'c':
				return;
			case 'i':
				{
					// parse the path into a vector of int (makes me miss C#)
					std::string programPath, temp;
					std::cin >> programPath;
					std::istringstream ss(programPath);
					std::vector<int> program;
					while(std::getline(ss, temp, '/'))
					{
						//std::cout << "'" << temp << "'" << std::endl;
						// first entry is empty string
						if(temp.size()) program.push_back(std::stoi(temp));
					}
					
					solver->dumpProgramInformation(program);
				}
				break;
			case 's':
				{
					// todo - choice of file name?
					std::string fileName = "tree.xml";
					std::cout << "Saving state to " << fileName << " ..." << std::endl;
					solver->save("tree.xml");
					std::cout << "Complete." << std::endl;
				}
				break;
			case 'q':
				solver->stop();
				return;
			default:
				std::cout << "Unknown command " << cmd << std::endl;
		}
	}
}