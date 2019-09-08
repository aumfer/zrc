#include <control.hpp>

void control::update(camera &camera, const ui &ui, const physics &physics, const map &map, flight &flight, caster &caster, float dt) {
	camera.zoom = float(ZOOM_DEFAULT - tanh(ui.scroll.y * zoom_rate) * ZOOM_DEFAULT);
	if (ui.keys[GLFW_KEY_LEFT] == GLFW_PRESS) {
		camera.position.x -= camera.zoom * scroll_rate * dt;
	}
	if (ui.keys[GLFW_KEY_RIGHT] == GLFW_PRESS) {
		camera.position.x += camera.zoom * scroll_rate * dt;
	}
	if (ui.keys[GLFW_KEY_UP] == GLFW_PRESS) {
		camera.position.y += camera.zoom * scroll_rate * dt;
	}
	if (ui.keys[GLFW_KEY_DOWN] == GLFW_PRESS) {
		camera.position.y -= camera.zoom * scroll_rate * dt;
	}

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

		if (ui.button(GLFW_MOUSE_BUTTON_2) == UI_RELEASED) {
			caster.get(select_entity, [&](caster_entity &caster_entity) {
				caster_entity.abilities.foreach([&](caster_ability &caster_ability) {
					caster_ability.target.point = glm::vec2(ui.pointer.x, ui.pointer.y);
					caster_ability.cast = true;
				});
			});
		}
	}
}