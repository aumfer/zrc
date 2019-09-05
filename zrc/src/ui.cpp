#include <ui.hpp>
#include <stdio.h>
#include <assert.h>

static int gl3w_init;
static int glfw_init;

static void ui_error_callback(int error, const char* description) {
	printf("glfw error %d: %s\n", error, description);
	assert(0);
}

static void ui_window_close_callback(GLFWwindow *window) {
	ui_t *ui = (ui_t *)glfwGetWindowUserPointer(window);
	ui->should_close = 1;
}

static void ui_window_size_callback(GLFWwindow *window, int width, int height) {
	ui_t *ui = (ui_t *)glfwGetWindowUserPointer(window);
	ui->window_size.x = width;
	ui->window_size.y = height;
	ui->window_aspect = (float)width / (float)height;
}

static void ui_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	ui_t *ui = (ui_t *)glfwGetWindowUserPointer(window);
	ui->framebuffer_size.x = width;
	ui->framebuffer_size.y = height;
	ui->framebuffer_aspect = (float)width / (float)height;
}

static void ui_scroll_callback(GLFWwindow *window, double x, double y) {
	ui_t *ui = (ui_t *)glfwGetWindowUserPointer(window);
	ui->scroll.x += x;
	ui->scroll.y += y;
}

ui::ui() {
	if (!glfw_init) {
		glfwSetErrorCallback(ui_error_callback);
		auto glfw_init_result = glfwInit();
		assert(glfw_init_result == GL_TRUE);
		glfw_init = 1;
	}

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//glfwWindowHint(GLFW_RED_BITS, 32);
	//glfwWindowHint(GLFW_GREEN_BITS, 32);
	//glfwWindowHint(GLFW_BLUE_BITS, 32);
	//glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(1920/2, 1080/2, "zen rat city", NULL, NULL);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowCloseCallback(window, ui_window_close_callback);
	glfwSetWindowSizeCallback(window, ui_window_size_callback);
	glfwSetFramebufferSizeCallback(window, ui_framebuffer_size_callback);
	//glfwSetKeyCallback(window, OnKey);
	//glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwSetScrollCallback(window, ui_scroll_callback);

	glfwGetWindowSize(window, &window_size.x, &window_size.y);
	window_aspect = (float)window_size.x / (float)window_size.y;

	glfwGetFramebufferSize(window, &framebuffer_size.x, &framebuffer_size.y);
	framebuffer_aspect = (float)framebuffer_size.x / (float)framebuffer_size.y;

	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);
	glfwSetTime(0);

	if (!gl3w_init) {
		gl3wInit();
		gl3w_init = 1;
	}
}
ui::~ui() {

}

void ui::update() {
	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; ++i) {
		prev_keys[i] = keys[i];
		keys[i] = glfwGetKey(window, i);
	}
	for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
		prev_mouse_buttons[i] = mouse_buttons[i];
		mouse_buttons[i] = glfwGetMouseButton(window, i);
	}

	glfwGetCursorPos(window, &pointer.x, &pointer.y);
	pointer.y = window_size.y - pointer.y;

	glfwSwapBuffers(window);
	glfwPollEvents();
}

ui_state ui::button(int button) const {
	const int *buttons, *prev_buttons;
	if (button < GLFW_MOUSE_BUTTON_LAST) {
		buttons = mouse_buttons;
		prev_buttons = prev_mouse_buttons;
	} else {
		buttons = keys;
		prev_buttons = prev_keys;
	}

	ui_state state = UI_INVALID;
	if (buttons[button]) {
		if (prev_buttons[button]) {
			state = UI_DOWN;
		} else {
			state = UI_PRESSED;
		}
	} else {
		if (prev_buttons[button]) {
			state = UI_RELEASED;
		} else {
			state = UI_UP;
		}
	}
	return state;
}