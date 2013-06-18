#include <cstdlib>

class IRandom
{
	public:
		virtual void init(int seed) = 0;
	
		// [0,max]
		virtual double getDouble(double max) = 0;
		
		// [0, max)
		virtual int getInt(int max) = 0;
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
			return rand() * max / (RAND_MAX - 1.0);
		}
		
		int getInt(int max)
		{
			return rand() % max;
		}
};