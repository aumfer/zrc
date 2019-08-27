#include <timer.h>
#include <GLFW/glfw3.h>

void timer_create(timer_t *timer) {
	timer->elapsed = 0;
	timer->time = (float)glfwGetTime();
	timer->prev = timer->time;
	timer->dt = 0;
}
void timer_destroy(timer_t *timer) {
}

void timer_update(timer_t *timer) {
	timer->prev = timer->time;
	timer->time = (float)glfwGetTime();
	timer->dt = timer->time - timer->prev;
	timer->elapsed += timer->dt;
}