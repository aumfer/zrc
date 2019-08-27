#ifndef _TIMER_H_
#define _TIMER_H_

typedef struct timer {
	float prev, time, elapsed, dt;
} timer_t;

void timer_create(timer_t *);
void timer_destroy(timer_t *);

void timer_update(timer_t *);

#endif