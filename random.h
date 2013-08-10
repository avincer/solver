#pragma once
#include <cstdlib>

class IRandom
{
	public:
		virtual void init(int seed) = 0;
	
		// [0,max]
		virtual double getDouble(double max) = 0;
		
		// [0, max)
		virtual int getInt(int max) = 0;
		
		// returns true or false at random weighted by chance
		// e.g. if chance = 0.9, returns true 9 times out of 10 (on average)
		virtual bool maybe(double chance) = 0;
};

class CStdRandom : public IRandom
{
	public:
		void init(int seed)
		{
			srand(seed);
		}
		
		double getDouble(double max)
		{
            return rand() * max / RAND_MAX;
		}
		
		int getInt(int max)
		{
			return rand() % max;
		}
		
		bool maybe(double chance)
		{
			return (rand() % RAND_MAX) < (int)(chance * RAND_MAX);
		}
};
