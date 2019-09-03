#ifndef _ZRC_H_
#define _ZRC_H_

#include <timer.h>
#include <moving_average.h>
#include <physics.h>
#include <ui.h>
#include <map.h>
#include <draw_map.h>
#include <font.h>
#include <curves.h>
#include <draw_curves.h>
#include <camera.h>
#include <visual.h>
#include <curves_map.h>
#include <draw_curves_map.h>
#include <camera_control.h>

typedef struct zrc {
	timer_t update_timer;
	timer_t draw_timer;
	moving_average_t fps;
	physics_t physics;
	ui_t ui;
	map_t map;
	draw_map_t draw_map;
	font_t font;
	curves_t curves;
	draw_curves_t draw_curves;
	camera_t camera;
	visual_t visual;
	curves_map_t curves_map;
	draw_curves_map_t draw_curves_map;
	camera_control_t camera_control;

	float accumulator;

	zrc();
	~zrc();
	void update();
	void draw();
} zrc_t;

#endif