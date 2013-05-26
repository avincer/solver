#include "solver.h"
#include "pile-up.h"
#include "tree.h"

#include <csignal>
#include <memory>
#include <fstream>
#include <fenv.h>

std::unique_ptr<Solver> solver;

void stop(int signal);

int main()
{
	// throw on floating point exceptions
	feenableexcept(FE_INVALID | FE_OVERFLOW);
	
	// todo - allow selection of the VM (and options)
	int stackSize = 2, memorySize = 16;
	std::unique_ptr<IVM> vm(new PileUp::VM(stackSize, memorySize));
	
	// todo - allow selection of random seed?
	auto seed = (int)time(nullptr);
	
	std::unique_ptr<IRandom> random(new CStdRandom());
	random->init(seed);
	
	// todo - allow setting initial weights
	std::vector<double> initialWeights(vm->getInstructionCount(), 0.5);
	
	// todo - allow selecting which program factory to use
	std::unique_ptr<IProgramFactory> factory(new ProgramTree(random.get(), initialWeights));
	
	// todo - get target sequence from somewhere!
	std::vector<float> target 
	{ 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31 };
	// { 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };
	
	// todo - get max ops from somewhere
	int maxOps = 1000;
	
	// terminate on user signal
	signal(SIGINT, stop);
	
	// build and run the solver (go put the kettle on...)
	solver.reset(new Solver(factory.get(), vm.get(), target));
	solver->run(maxOps);
	
	// save progress
	//std::ofstream file("tree.xml");
	//factory->toXml(file);
	
	return 0;
}

void stop(int signal)
{
	solver->stop();
}