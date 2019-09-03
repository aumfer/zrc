#ifndef _UI_H_
#define _UI_H_

#include <lsgl.h>
#include <GLFW/glfw3.h>

typedef struct ui {
	GLFWwindow *window;
	struct {
		double x, y;
	} pointer;
	struct {
		double x, y;
	} scroll;
	struct {
		int x, y;
	} window_size;
	float window_aspect;
	struct {
		int x, y;
	} framebuffer_size;
	float framebuffer_aspect;
	int should_close;

	int keys[GLFW_KEY_LAST];
	int prev_keys[GLFW_KEY_LAST];
	int mouse_buttons[GLFW_MOUSE_BUTTON_LAST];
	int prev_mouse_buttons[GLFW_MOUSE_BUTTON_LAST];

	ui();
	~ui();
	void update();
} ui_t;

#endif