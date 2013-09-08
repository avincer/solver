#include "solver.h"

#include <iostream>
#include <iomanip>

const char* Solver::version = "0.2";

double Solver::scoreOutput(int outputLen)
{
	// todo - consider whether factoring in the length of the 
	// program is the correct approach
	double score = 0;
	for(int i = 0; i < outputLen; ++i)
	{
		double e = output[i] - target[i];
		score += 1.0 / (1.0 + e * e);
	}
	return score / target.size();
}

void Solver::updateBestPrograms(ProgramInfo& program, int outputLen)
{
	if(bestPrograms.size() < bestProgramCount || 
		program.score > bestPrograms.back().score)
	{
		// store the program along with its output
		program.output = output;
		program.output.resize(outputLen);
		
		if(bestPrograms.size() == 0)
		{
			// first program to be stored!
			bestPrograms.push_back(program);
		}
		else
		{
			// insert the program in the correct place
			// todo - maybe use a set for this?
			bool inserted = false;
			for(auto it = bestPrograms.begin(); 
				it != bestPrograms.end() && !inserted; ++it)
			{
				if(program.score > it->score)
				{
					bestPrograms.insert(it, program);
					inserted = true;
				}
			}
			
			if(bestPrograms.size() > bestProgramCount)
			{
				bestPrograms.pop_back();
			}
		}
	}
}

void Solver::outputStatus()
{
	std::cout << " solver: v" << version << std::endl;
	std::cout << std::endl;
	std::cout << "factory: " << factory->getName() << std::endl;
	std::cout << "     vm: " << vm->getName() << std::endl;
	std::cout << " target: ";
	outputSequence(target);
	
	double runTime, speed;
	timer.update(programCount, runTime, speed);
	
	std::cout << " status: tested " << programCount << " programs";
	std::cout << " (" << (int)speed << " programs/second)" << std::endl;
	
	auto hours = (int)(runTime / 3600);
	runTime -= hours * 3600;
	auto minutes = (int)(runTime / 60);
	runTime -= minutes * 60;
	
	std::cout << "   time: " << hours << ":";
	std::cout << std::setfill('0');
	std::cout << std::setw(2) << minutes << ":";
	std::cout << std::setw(2) << (int)runTime;
	std::cout << std::setfill(' ') << std::endl;
	
	std::cout << std::endl;
	std::cout << " score | output" << std::endl;
	for(auto programInfo: bestPrograms)
	{
		std::cout << std::setw(6) << std::setprecision(3) << programInfo.score;
		std::cout << std::setw(0) << " | ";
		outputSequence(programInfo.output);
		std::cout << "       | " << 
			vm->returnStringForm(programInfo.program) << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Solver::outputSequence(const std::vector<float>& sequence)
{
	// todo - add support for wrapping
	for(auto x: sequence)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
}

Solver::Solver(IProgramFactory* factory, IVM* vm, 
		const std::vector<float>& target)
{
	this->factory = factory;
	this->vm = vm;
	this->target = target;
	output.resize(target.size());
	programCount = 0;
}

void Solver::run(size_t maxPrograms, bool exitOnFirstSolution, double brevityWeight)
{
	running = true;
	auto remainingPrograms = maxPrograms;
	timer.start();
	
	while(running)
	{
		// create a program and load it into the VM
		ProgramInfo programInfo = factory->createNewProgram();
		vm->loadProgram(programInfo.program);
		
		// run the program and store output
		bool result = true;
		int i = 0;
		for(; (i < target.size()) && result; ++i)
		{
			result = vm->run(i, &output[i]);
		}
		
		// score the program and update the program factory stats
		auto outputScore = scoreOutput(i);
		auto brevityScore = 1.0 / programInfo.program.size();
		programInfo.score = (1.0 - brevityWeight) * outputScore + brevityWeight * brevityScore;
		factory->recordProgramScore(programInfo);
		
		// record it if it was any good
		updateBestPrograms(programInfo, i);

		++programCount;

		// output status every so often
		if(programCount % updatePeriod == 0) outputStatus();

		if(maxPrograms)
		{
			--remainingPrograms;
			if(!remainingPrograms) running = false;
		}
		
		if(exitOnFirstSolution && outputScore == 1) running = false;
		
		if(!running) outputStatus();
	}
}

void Solver::stop()
{
	running = false;
}

void Solver::dumpProgramInformation(const Program& program)
{
	factory->dumpProgramInformation(program);
}
