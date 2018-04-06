#include<time.h>

#ifndef TIMER_H
#define TIMER_H

class timer {
	private:
		clock_t starting_time;	//	the starting time of the current timer
	public:
		void start();			// set the starting time to now
		int time_passed();		// return the time passed since start in milisecond
		timer() {}				// constructor
};

void timer::start(){
	starting_time = clock();
}

int timer::time_passed(){
	return 1000. * (clock() - starting_time) / CLOCKS_PER_SEC;
}

#endif
