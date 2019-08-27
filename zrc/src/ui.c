#include <ui.h>
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

void ui_create(ui_t *ui) {
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
	//glfwWindowHint(GLFW_SAMPLES, 32);

	ui->window = glfwCreateWindow(1920/2, 1080/2, "zen rat city", NULL, NULL);

	glfwSetWindowUserPointer(ui->window, ui);
	glfwSetWindowCloseCallback(ui->window, ui_window_close_callback);
	glfwSetWindowSizeCallback(ui->window, ui_window_size_callback);
	glfwSetFramebufferSizeCallback(ui->window, ui_framebuffer_size_callback);
	//glfwSetKeyCallback(window, OnKey);
	//glfwSetMouseButtonCallback(window, OnMouseButton);
	//glfwSetScrollCallback(ui->window, OnScroll);

	glfwGetWindowSize(ui->window, &ui->window_size.x, &ui->window_size.y);
	ui->window_aspect = (float)ui->window_size.x / (float)ui->window_size.y;

	glfwGetFramebufferSize(ui->window, &ui->framebuffer_size.x, &ui->framebuffer_size.y);
	ui->framebuffer_aspect = (float)ui->framebuffer_size.x / (float)ui->framebuffer_size.y;

	glfwMakeContextCurrent(ui->window);

	glfwSwapInterval(0);
	glfwSetTime(0);

	if (!gl3w_init) {
		gl3wInit();
		gl3w_init = 1;
	}
}
void ui_destroy(ui_t *ui) {

}

void ui_update(ui_t *ui) {
	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; ++i) {
		ui->prev_keys[i] = ui->keys[i];
		ui->keys[i] = glfwGetKey(ui->window, i);
	}
	for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
		ui->prev_mouse_buttons[i] = ui->mouse_buttons[i];
		ui->mouse_buttons[i] = glfwGetMouseButton(ui->window, i);
	}

	glfwGetCursorPos(ui->window, &ui->pointer.x, &ui->pointer.y);

	glfwSwapBuffers(ui->window);
	glfwPollEvents();
}