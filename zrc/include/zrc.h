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
#include <spines.hpp>
#include <draw_spines.hpp>

typedef struct zrc {
	timer update_timer;
	timer draw_timer;
	moving_average fps;
	physics physics;
	ui ui;
	map map;
	draw_map draw_map;
	font font;
	curves curves;
	draw_curves draw_curves;
	camera camera;
	visual visual;
	curves_map curves_map;
	draw_curves_map draw_curves_map;
	camera_control camera_control;
	spines spines;
	draw_spines draw_spines;

	float accumulator;

	zrc();
	~zrc();
	void update();
	void draw();
} zrc_t;

#endif