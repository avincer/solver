#include <ctime>

class Timer
{
	private:
		clock_t then;
		
	public:
		// begin timing
		void start()
		{
			then = clock();
		}
		
		// get the elapsed time in seconds
		// start time is reset if reset is true
		double getElapsedTime(bool reset)
		{
			clock_t now = clock();
			auto result = (now - then) / (double)CLOCKS_PER_SEC;
			if(reset) then = now;
			return result;
		}
};