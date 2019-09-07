#include <control.hpp>

void control::update(camera &camera, const ui &ui, const physics &physics, const map &map, flight &flight, float dt) {
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

	if (select_entity != ID_EMPTY) {
		flight.get(select_entity, [&](flight_entity &flight_entity) {
			glm::vec2 thrust = glm::vec2();
			float turn = 0;
			if (ui.keys[GLFW_KEY_W]) {
				thrust.y += 1;
			}
			if (ui.keys[GLFW_KEY_S]) {
				thrust.y -= 1;
			}
			if (ui.keys[GLFW_KEY_A]) {
				turn += 1;
			}
			if (ui.keys[GLFW_KEY_D]) {
				turn -= 1;
			}

			flight_entity.thrust = thrust;
			flight_entity.turn = turn;
		});

		physics.get(select_entity, [&](const physics_entity &physics_entity) {
			int found = 0;
			map.query_point(physics_entity.position, 64, [&](id id) {
				if (id != physics_entity.id) {
					++found;
				}
			});
			int found2 = 0;
			physics.query_point(physics_entity.position, 64, [&](const physics_entity_t &near_entity) {
				if (near_entity.id != physics_entity.id) {
					++found2;
				}
			});
			physics.query_ray(physics_entity.position, physics_entity.position + physics_entity.velocity, 1, [&](const physics_entity_t &near_entity, const glm::vec2 &point) {
			});
			if (found || found2) {
				printf("found %d %d\n", found, found2);
			}
		});
	}
}