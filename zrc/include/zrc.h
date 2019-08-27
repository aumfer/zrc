#ifndef _ZRC_H_
#define _ZRC_H_

#include <timer.h>
#include <moving_average.h>
#include <physics.h>
#include <ui.h>
#include <map.h>
#include <draw.h>
#include <font.h>
#include <lines.h>
#include <draw_lines.h>
#include <curves.h>
#include <draw_curves.h>
#include <camera.h>
#include <visual.h>
#include <curves_map.h>
#include <lines_map.h>
#include <draw_lines_map.h>
#include <draw_curves_map.h>

typedef struct zrc {
	timer_t timer;
	moving_average_t fps;
	physics_t physics;
	ui_t ui;
	map_t map;
	draw_t draw;
	font_t font;
	lines_t lines;
	draw_lines_t draw_lines;
	curves_t curves;
	draw_curves_t draw_curves;
	camera_t camera;
	visual_t visual;
	curves_map_t curves_map;
	lines_map_t lines_map;
	draw_lines_map_t draw_lines_map;
	draw_curves_map_t draw_curves_map;

	float accumulator;
} zrc_t;

void zrc_create(zrc_t *);
void zrc_destroy(zrc_t *);

void zrc_update(zrc_t *);

#endif