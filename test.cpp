#include "tree.h"
#include "pile-up.h"

#include <iostream>
#include <cassert>
#include <map>

using namespace PileUp;

void testRandomDistribution()
{
	std::cout << "Testing random distribution ... ";
	
	const int binCount = 100;
	const int testsPerBin = 100;
	const int threshold = 30;
	
	int bins[binCount];
	for(int i = 0; i < binCount; ++i)
	{
		bins[i] = 0;
	}
	
	CStdRandom random;
	random.init(1337);
	
	for(int i = 0; i < binCount * testsPerBin; ++i)
	{
		bins[(int)random.getDouble(binCount)]++;
	}
	
	for(int i = 0; i < binCount; ++i)
	{
		// todo: there are much better and more rigorous ways of checking this!
		if(bins[i] < testsPerBin - threshold || bins[i] > testsPerBin + threshold)
		{
			std::cout << "Failed!" << std::endl;
			std::cout << "Random distribution may not be flat - got " << bins[i] << " in bin " << i << std::endl;
			return;
		}
	}
	
	std::cout << "OK" << std::endl;
}

void testNodeScoring()
{
	std::cout << "Testing node scoring ... ";
	
	std::vector<double> weights(InstructionCount, 1.0);
	
	Node* root = new Node(0, nullptr, weights);
	
	// check child links off root node were set properly
	for(int i = 0; i < InstructionCount; ++i)
	{
		NodeLink link = root->children[i];
		assert(link.node == nullptr);
		assert(link.weight == 1);
	}
	
	// add a child node (a) to root in position 0 with score 3
	Node* a = root->children[0].node = new Node(0, root, weights);
	a->setScore(3);
	
	// we now expect that the link to (a) has weight 3 
	// because (a) has no children
	NodeLink link = root->children[0];	
	assert(link.weight == 3);
	
	// add a child node (b) to (a) in position 1 with score 4
	Node* b = a->children[1].node = new Node(1, a, weights);
	b->setScore(4);
	
	// we expect the weight for (a) to be (3+4)/2 = 3.5
	link = root->children[0];	
	assert(link.weight == 3.5);
	
	// add a new node (c) under (b)
	Node* c = b->children[0].node = new Node(0, b, weights);
	c->setScore(1);
	
	// and another node (d) under (a) in position 2
	Node* d = a->children[2].node = new Node(2, a, weights);
	d->setScore(8);
	
	// expect weight for a = (s + S) / (N+1)
	// s = score for a (= 3)
	// S = total score for descendants of a (= 4 + 1 + 8)
	// N = total descendants of a (= 3)
	// => expected weight of 4
	assert(root->children[0].weight == 4);

	std::cout << "OK" << std::endl;
}

void testInstructionSelection()
{
	std::cout << "Testing instruction selection ... ";
	
	std::vector<double> weights(InstructionCount, 0);
	weights[3] = 1;
	weights[5] = 3;
	
	// expect roughly 3x as many (5) instructions as (3) instructions
	// expect no instructions of any other type!

	int bins[InstructionCount];
	for(int i = 0; i < InstructionCount; ++i)
	{
		bins[i] = 0;
	}
	
	CStdRandom random;
	random.init(1337);
	ProgramTree tree(&random, weights, Directed);
	auto node = (Node*)tree.createNewProgram().node;
	
	for(int i = 0; i < 1000; ++i)
	{
		// expect instruction to be in range!
		int instruction = tree.chooseNextInstruction(node);
		assert(instruction >= 0);
		assert(instruction < InstructionCount);
		
		bins[instruction]++;
	}
	
	for(int i = 0; i < InstructionCount; ++i)
	{
		if(i != 3 && i != 5)
		{
			assert(bins[i] == 0);
		}
	}
	
	double ratio = bins[5] / (double)bins[3];
	assert(ratio > 2.9);
	assert(ratio < 3.1);
	
	std::cout << "OK" << std::endl;
}

int main()
{
	std::cout << "sizeof(int) == " << sizeof(int) << std::endl;
	std::cout << "sizeof(Node) == " << sizeof(Node) << std::endl;
	std::cout << "sizeof(NodeLink) == " << sizeof(NodeLink) << std::endl;
	std::cout << "sizeof(std::vector<NodeLink>) == " << sizeof(std::vector<NodeLink>) << std::endl;
	std::cout << "sizeof(std::map<int, NodeLink>) == " << sizeof(std::map<int, NodeLink>) << std::endl;
	
	testRandomDistribution();
	testNodeScoring();
	testInstructionSelection();
}