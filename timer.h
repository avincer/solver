#include <ctime>

class Timer
{
	private:
		clock_t startTime, lastUpdateTime;
		size_t lastCount;
		double lastSpeed;
		
	public:
		void start()
		{
			startTime = lastUpdateTime = clock();
			lastCount = 0;
			lastSpeed = 0;
		}
		
		void update(size_t count, double& totalTime, double& speed)
		{
			clock_t now = clock();
			
			totalTime = (now - startTime) / (double)CLOCKS_PER_SEC;
			
			auto dCount = count - lastCount;
			auto dTime = (now - lastUpdateTime) / (double)CLOCKS_PER_SEC;
			speed = (dCount > 0 && dTime > 0) ? dCount / dTime : lastSpeed;
			
			lastUpdateTime = now;
			lastCount = count;
			lastSpeed = speed;
		}
};