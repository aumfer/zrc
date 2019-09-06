#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

void camera::update(const ui &ui) {
	float extentx = zoom / 2;
	float extenty = extentx / ui.framebuffer_aspect;
	projection = glm::ortho(-extentx, +extentx, -extenty, +extenty);
	view = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0));

	//projection = glm::perspectiveFov(90.0f, float(ui.framebuffer_size.x), float(ui.framebuffer_size.y), 0.1f, 1000.0f);
	//view = glm::lookAt(glm::vec3(position, extenty), glm::vec3(), glm::vec3(0, 0, 1));

	view_projection = projection * view;
}
