#include <control.hpp>

void control::update(camera &camera, const ui &ui, const physics &physics, float dt) {
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

	glm::vec4 viewport = glm::vec4(0, 0, ui.window_size.x, ui.window_size.y);
	pick_start = glm::unProject(glm::vec3(ui.pointer.x, ui.pointer.y, 0), camera.view, camera.projection, viewport);
	pick_end = glm::unProject(glm::vec3(ui.pointer.x, ui.pointer.y, 1), camera.view, camera.projection, viewport);

	float pick_radius = 1;
	physics_entity *pick_physics = physics.query_ray(pick_start, pick_end, pick_radius);
	if (pick_physics) {
		hover_entity = pick_physics->id;
	} else {
		hover_entity = ID_EMPTY;
	}

	if (ui.button(GLFW_MOUSE_BUTTON_1) == UI_RELEASED) {
		select_entity = hover_entity;
	}
}