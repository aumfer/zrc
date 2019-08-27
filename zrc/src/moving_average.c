#include <moving_average.h>
#include <stdlib.h>

void moving_average_create(moving_average_t *moving_average) {
}
void moving_average_destroy(moving_average_t *moving_average) {
}

void moving_average_update(moving_average_t *moving_average, float value) {
	moving_average_t *ma = moving_average;
	float sum = 0;

	ma->buffer[ma->index++ & MOVING_AVERAGE_MASK] = value;
	unsigned count = ma->index > MOVING_AVERAGE_COUNT ? MOVING_AVERAGE_COUNT : ma->index;
	for (unsigned i = 0; i < count; ++i) {
		sum += ma->buffer[i];
	}
	ma->avg = sum / count;
}