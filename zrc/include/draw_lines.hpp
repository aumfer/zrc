#pragma once

#include <vector_display.h>
#include <ui.hpp>
#include <camera.hpp>
#include <visual.hpp>

typedef struct draw_lines {
	vector_display *vector_display;

	draw_lines();
	~draw_lines();
	void update(const ui &, const camera &, const visual &);
} draw_lines_t;