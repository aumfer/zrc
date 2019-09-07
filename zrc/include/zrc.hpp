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
#include <control.hpp>
#include <spines.hpp>
#include <draw_spines.hpp>
#include <flight.hpp>
#include <locomotion.hpp>
#include <seek.hpp>

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
	control control;
	spines spines;
	draw_spines draw_spines;
	flight flight;
	locomotion locomotion;
	seek seek;

	float accumulator;

	zrc();
	~zrc();
	void update();
	void draw();
} zrc_t;

#endif