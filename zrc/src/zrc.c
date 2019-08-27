#include <zrc.h>
#include <stdio.h>

const float TICK_RATE = 1.0f / 60.0f;

void zrc_create(zrc_t *zrc) {
	timer_create(&zrc->timer);
	moving_average_create(&zrc->fps);
	physics_create(&zrc->physics);
	ui_create(&zrc->ui);
	map_create(&zrc->map);
	draw_create(&zrc->draw);
	font_create(&zrc->font, FONT_CONSOLAS_16);
	lines_create(&zrc->lines);
	draw_lines_create(&zrc->draw_lines);
	curves_create(&zrc->curves);
	draw_curves_create(&zrc->draw_curves);
	camera_create(&zrc->camera);
	visual_create(&zrc->visual);
	curves_map_create(&zrc->curves_map);
	lines_map_create(&zrc->lines_map);
	draw_lines_map_create(&zrc->draw_lines_map);
	draw_curves_map_create(&zrc->draw_curves_map);
}
void zrc_destroy(zrc_t *zrc) {
	timer_destroy(&zrc->timer);
	moving_average_destroy(&zrc->fps);
	physics_destroy(&zrc->physics);
	ui_destroy(&zrc->ui);
	map_destroy(&zrc->map);
	draw_destroy(&zrc->draw);
	font_destroy(&zrc->font);
	lines_destroy(&zrc->lines);
	draw_lines_destroy(&zrc->draw_lines);
	curves_destroy(&zrc->curves);
	draw_curves_destroy(&zrc->draw_curves);
	camera_destroy(&zrc->camera);
	visual_destroy(&zrc->visual);
	curves_map_destroy(&zrc->curves_map);
	lines_map_destroy(&zrc->lines_map);
	draw_lines_map_destroy(&zrc->draw_lines_map);
	draw_curves_map_destroy(&zrc->draw_curves_map);
}

void zrc_update(zrc_t *zrc) {
	ui_update(&zrc->ui);

	timer_update(&zrc->timer);
	moving_average_update(&zrc->fps, zrc->timer.dt);
	zrc->accumulator += zrc->timer.dt;
	while (zrc->accumulator >= TICK_RATE) {
		zrc->accumulator -= TICK_RATE;
		physics_update(&zrc->physics, TICK_RATE);
		map_update(&zrc->map);
	}

	camera_update(&zrc->camera, &zrc->ui);
	visual_update(&zrc->visual, &zrc->physics, zrc->accumulator);

	draw_update(&zrc->draw, &zrc->ui, &zrc->camera, &zrc->curves_map);

	lines_update(&zrc->lines, &zrc->visual);
	draw_lines_update(&zrc->draw_lines, &zrc->lines, &zrc->ui, &zrc->camera);
	//
	//lines_map_update(&zrc->lines_map, &zrc->lines);
	//draw_lines_map_update(&zrc->draw_lines_map, &zrc->lines, &zrc->lines_map);

	curves_update(&zrc->curves, &zrc->visual);
	//draw_curves_update(&zrc->draw_curves, &zrc->curves, &zrc->ui, &zrc->camera);

	curves_map_update(&zrc->curves_map, &zrc->visual);
	draw_curves_map_update(&zrc->draw_curves_map, &zrc->curves, &zrc->curves_map);

	font_begin(&zrc->font);
	char fps[32];
	sprintf_s(fps, sizeof(fps), "%.2fms %.0ffps", zrc->fps.avg, 1 / zrc->fps.avg);
	font_print(&zrc->font, fps, (GLvec2) { 11, 10 }, (GLubvec4) { 51, 51, 51, 255 });
	font_print(&zrc->font, fps, (GLvec2) { 10, 11 }, (GLubvec4) { 204, 204, 204, 255 });
	font_draw(&zrc->font, &zrc->ui);
	font_end(&zrc->font);
}