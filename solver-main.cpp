#include "solver.h"
#include "pile-up.h"
#include "newtable.h"
#include "append-factory.h"
#include "top-factory.h"

#include <csignal>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>

// os specific headers
// note: can dump preprocessor macros for gcc using gcc -dM -E - < /dev/null
// gcc defines linux, __linux and __linux__
// s/o suggests using __linux
// http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#ifdef __linux
#include <fenv.h>
#endif

#define option(_arg, _type, _var, _help) (_arg, po::value<_type>(&options._var)->default_value(options._var), _help)

std::unique_ptr<Solver> solver;

// storage for program options and capabilities
typedef struct
{
	// capabilities (name -> description)
	std::map<std::string, std::string> vmList;
	std::map<std::string, std::string> factoryList;

	// program settings
	std::string vm;
	std::string factory;
	int randomSeed;
	
	// todo - add support for setting individual instruction weights
	double instructionInitialWeight;
	double brevityWeight;
	
	std::string target;
	std::string targetFile;
	
	// factory specific settings
	size_t topFactoryMaxPrograms;
	double topFactoryExplorationChance;
	
	// vm specific settings
	int pileUpStackSize;
	int pileUpMemorySize;
	int pileUpMaxOps;
	
	unsigned int newtableMostRunTimePerOutput;
}
SolverOptions;

// forward declarations
bool parseCSV(std::string csv, std::vector<float>& sequence);
bool parseCSVFile(std::string fileName, std::vector<float>& sequence);
bool parseOptions(int argc, char** argv, SolverOptions& options);
void pause(int signal);

int main(int argc, char** argv)
{
	// set option defaults
	SolverOptions options;
	
	options.vmList.emplace("pile-up", "virtual machine using stack based arithmetic");
	options.vmList.emplace("newtable", "");

	options.factoryList.emplace("append", "Monte Carlo tree search (append instruction mode)");
	options.factoryList.emplace("random", "Random program generation");
	options.factoryList.emplace("top", "Random generation and mutation of top programs");
	
	options.vm = "pile-up";
	options.factory = "append";
	options.randomSeed = -1;
	options.instructionInitialWeight = 0.5;
	options.brevityWeight = 0.2;
	
	options.topFactoryMaxPrograms = 100;
	options.topFactoryExplorationChance = 0.5;

	options.pileUpStackSize = 16;
	options.pileUpMemorySize = 16;
	options.pileUpMaxOps = 1000;
	
	options.newtableMostRunTimePerOutput = 100;
	
	if(!parseOptions(argc, argv, options)) return 0;
	
	// todo - find windows equivalent of this
#ifdef __linux
	// throw on floating point exceptions
	feenableexcept(FE_DIVBYZERO | FE_INVALID);
#endif
	
	std::unique_ptr<IVM> vm;
	if(options.vm == "pile-up")
	{
		vm.reset(new PileUp::VM(options.pileUpStackSize, options.pileUpMemorySize, options.pileUpMaxOps));
	}
	else
	{
		vm.reset(new newtAble::VM(options.newtableMostRunTimePerOutput));
	}
	
	auto seed = options.randomSeed == -1 ? (int)time(nullptr) : options.randomSeed;
	
	std::unique_ptr<IRandom> random(new CStdRandom());
	random->init(seed);
		
	std::unique_ptr<IProgramFactory> factory;
	if(options.factory == "append")
	{
		std::vector<double> initialWeights(vm->supportedInstructionCount(), options.instructionInitialWeight);
		factory.reset(new AppendFactory(random.get(), initialWeights));
	}
	else if(options.factory == "random")
	{
		factory.reset(new RandomFactory(random.get(), vm->supportedInstructionCount()));
	}
	else
	{
		factory.reset(new TopFactory(random.get(), vm->supportedInstructionCount(),
			options.topFactoryMaxPrograms, options.topFactoryExplorationChance));
	}		

	std::vector<float> target;
	if(!options.target.empty())
	{
		if(!parseCSV(options.target, target)) return 0;
	}
	else
	{
		if(!parseCSVFile(options.targetFile, target)) return 0;
	}
	
	// terminate on user signal
	signal(SIGINT, pause);
	
	// suspect we always want to exit once a solution has been found
	// todo - parameterise if required
	auto exitOnFirstSolution = true;
	
	// build and run the solver (go put the kettle on...)
	solver.reset(new Solver(factory.get(), vm.get(), target));
	solver->run(0 /*10000000*/, exitOnFirstSolution, options.brevityWeight);
	
	return 0;
}

bool parseCSV(std::string csv, std::vector<float>& sequence)
{
	std::istringstream ss(csv);
	try
	{
		for(std::string str; std::getline(ss, str, ','); sequence.push_back(std::stof(str)));
	}
	catch(const std::exception& ex)
	{
		std::cout << "Failed to parse target sequence - " << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool parseCSVFile(std::string fileName, std::vector<float>& sequence)
{
	std::ifstream file(fileName);
	for(std::string line; std::getline(file, line); )
	{
		if(line[0] != '#')
		{
			if(!parseCSV(line, sequence)) return false;
		}
	}
	return true;
}

// parses command line options, returning true if the program should continue
bool parseOptions(int argc, char** argv, SolverOptions& options)
{
	namespace po = boost::program_options;
	
	po::options_description optionList("Solver options");
	optionList.add_options()
		("help", "Print this message.")
		("version", "Print program version.")
		
		option("vm", std::string, vm, "Selects the virtual machine to use. Passing list will print supported VMs.")
		option("factory", std::string, factory, "Selects the program factory to use. Passing list will print supported factories.")
		option("seed", int, randomSeed, "Sets the RNG seed. -1 selects a seed at runtime.")
		option("weight", double, instructionInitialWeight, "Sets the initial instruction weight. Should be between 0 and 1")
		option("target", std::string, target, "Sets the target sequence.")
		option("target-file", std::string, targetFile, "Loads the target sequence from a file.")
		option("brevity-weight", double, brevityWeight, "Sets the bias towards shorter programs. Should be between 0 and 1")
		
		option("top-factory-max-programs", size_t, topFactoryMaxPrograms, "Sets number of top programs to store in top factory.")
		option("top-factory-exploration-chance", double, topFactoryExplorationChance, "Sets chance of exploring new program (vs mutating existing program).")
		
		option("pile-up-stack-size", int, pileUpStackSize, "Sets stack size for the pile-up VM.")
		option("pile-up-memory-size", int, pileUpMemorySize, "Sets memory size for the pile-up VM.")
		option("pile-up-max-ops", int, pileUpMaxOps, "Sets maximum operation count for the pile-up VM.")

		option("newtable-most-run-time-per-output", unsigned int, newtableMostRunTimePerOutput, "Sets most run time per output.")
		;
	
	po::variables_map args;

	try
	{
		po::store(po::parse_command_line(argc, argv, optionList), args);
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return false;
	}
	
	// maybe this has something to do with checking args?
	po::notify(args);
	
	if (args.count("help"))
	{
		std::cout << optionList << std::endl;
		return false;
	}
	
	if (args.count("version"))
	{
		std::cout << "Solver v" << Solver::version << std::endl;
		return false;
	}
	
	auto checkListOption = [&args](std::string option, std::map<std::string, std::string> list)
	{
		if(args.count(option))
		{
			auto arg = args[option].as<std::string>();
			if(arg == "list")
			{
				// list the things we support
				for(auto thing: list)
				{
					std::cout << thing.first << " : " << thing.second << std::endl;
				}
				return false;
			}
			else
			{
				// check that the selected thing is valid
				for(auto thing: list)
				{
					if(thing.first == arg) return true;
				}
				std::cout << arg << " is not a valid value for " << option << std::endl;
				return false;
			}
		}
		return true;
	};
	
	if(!checkListOption("vm", options.vmList)) return false;
	if(!checkListOption("factory", options.factoryList)) return false;

	// we require exactly one of target or targetFile
	int targetOptions = (int)(!options.target.empty()) + (int)(!options.targetFile.empty());
	if(targetOptions != 1)
	{
		std::cout << "Please specify target sequence using --target or --target-file (but not both!)" << std::endl;
		return false;
	}

	return true;
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
					Program program;
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
