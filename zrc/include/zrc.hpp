#ifndef _ZRC_H_
#define _ZRC_H_

#include <timer.hpp>
#include <moving_average.hpp>
#include <physics.hpp>
#include <ui.hpp>
#include <map.hpp>
#include <draw_map.hpp>
#include <font.hpp>
#include <curves.hpp>
#include <draw_curves.hpp>
#include <camera.hpp>
#include <visual.hpp>
#include <curves_map.hpp>
#include <draw_curves_map.hpp>
#include <control.hpp>
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
	control control;
	spines spines;
	draw_spines draw_spines;

	float accumulator;

	zrc();
	~zrc();
	void update();
	void draw();
} zrc_t;

#endif