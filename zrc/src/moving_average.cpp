#include <moving_average.hpp>
#include <stdlib.h>

void moving_average::update(float value) {
	float sum = 0;

	buffer[index++ & MOVING_AVERAGE_MASK] = value;
	unsigned count = index > MOVING_AVERAGE_COUNT ? MOVING_AVERAGE_COUNT : index;
	for (unsigned i = 0; i < count; ++i) {
		sum += buffer[i];
	}
	avg = sum / count;
}