#include <timer.hpp>
#include <time.h>

timer::timer() {
	elapsed = 0;
	time = clock() / (float)CLOCKS_PER_SEC;
	prev = time;
	dt = 0;
}
timer::~timer() {
}

void timer::update() {
	prev = time;
	time = clock() / (float)CLOCKS_PER_SEC;
	dt = time - prev;
	elapsed += dt;
}