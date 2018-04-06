#include<time.h>

#ifndef TIMER_H
#define TIMER_H

class timer {
	private:
		clock_t starting_time;
	public:
		void start();
		int time_passed();
};

void timer::start(){
	starting_time = clock();
}

int timer::time_passed(){
	return clock() - starting_time;
}

#endif
