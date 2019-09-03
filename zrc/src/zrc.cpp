#include <zrc.h>
#include <stdio.h>
#include <glm/gtx/string_cast.hpp>

const float TICK_RATE = 1.0f / 60.0f;

zrc::zrc() {
}
zrc::~zrc() {
}

void zrc::update() {
	update_timer.update();
	accumulator += update_timer.dt;
	while (accumulator >= TICK_RATE) {
		accumulator -= TICK_RATE;
		physics.update(TICK_RATE);
		map.update();
	}
}

void zrc::draw() {
	ui.update();

	draw_timer.update();
	fps.update(draw_timer.dt);

	camera_control.update(camera, ui, draw_timer.dt);

	camera.update(ui);
	visual.update(physics, accumulator);

	draw_map.update(ui, camera, curves_map, curves);
	curves.update(visual);
	draw_curves.update(curves, ui, camera);
	//curves_map_update(&zrc->curves_map, &zrc->visual);
	//draw_curves_map_update(&zrc->draw_curves_map, &zrc->curves, &zrc->curves_map);

	font.begin();
	char fps_str[32];
	sprintf_s(fps_str, sizeof(fps_str), "%.2fms %.0ffps", fps.avg, 1 / fps.avg);
	font.print(FONT_CONSOLAS_16, fps_str, glm::value_ptr(glm::vec2(11, 10)), glm::value_ptr(glm::u8vec4(51, 51, 51, 255)));
	font.print(FONT_CONSOLAS_16, fps_str, glm::value_ptr(glm::vec2(10, 11)), glm::value_ptr(glm::u8vec4(204, 204, 204, 255)));
	char pos_str[32];
	sprintf_s(pos_str, sizeof(pos_str), "%.2f %2.f", camera.position.x, camera.position.y);
	//const char *pos_str = glm::to_string(camera.position).c_str();
	font.print(FONT_CONSOLAS_16, pos_str, glm::value_ptr(glm::vec2(11, 30)), glm::value_ptr(glm::u8vec4(51, 51, 51, 255)));
	font.print(FONT_CONSOLAS_16, pos_str, glm::value_ptr(glm::vec2(10, 31)), glm::value_ptr(glm::u8vec4(204, 204, 204, 255)));
	font.draw(ui);
	font.end();
}