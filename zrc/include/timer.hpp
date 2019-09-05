#ifndef _TIMER_H_
#define _TIMER_H_

typedef struct timer {
	float prev, time, elapsed, dt;

	timer();
	~timer();
	void update();
} timer_t;

#endif