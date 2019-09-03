#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>

void camera::update(const ui &ui) {
	float extentx = zoom / 2;
	float extenty = extentx / ui.framebuffer_aspect;
	projection = glm::ortho(position.x - extentx, position.x + extentx, position.y - extenty, position.y + extenty);
	view = glm::mat4();
}
