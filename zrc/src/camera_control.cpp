#include <camera_control.h>

void camera_control::update(camera &camera, const ui &ui, float dt) {
	if (ui.keys[GLFW_KEY_LEFT] == GLFW_PRESS) {
		camera.position.x -= scroll_rate * dt;
	}
	if (ui.keys[GLFW_KEY_RIGHT] == GLFW_PRESS) {
		camera.position.x += scroll_rate * dt;
	}
	if (ui.keys[GLFW_KEY_UP] == GLFW_PRESS) {
		camera.position.y += scroll_rate * dt;
	}
	if (ui.keys[GLFW_KEY_DOWN] == GLFW_PRESS) {
		camera.position.y -= scroll_rate * dt;
	}
	camera.zoom = float(ZOOM_DEFAULT - ui.scroll.y * zoom_rate);
}