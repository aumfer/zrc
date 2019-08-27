#include <camera.h>

void camera_create(camera_t *camera) {
}
void camera_destroy(camera_t *camera) {
}

void camera_update(camera_t *camera, const ui_t *ui) {
	float extenty = WORLD_HALF * camera->zoom;
	float extentx = extenty * ui->framebuffer_aspect;
	camera->projection = HMM_Orthographic(camera->position.X - extentx, camera->position.X + extentx, camera->position.Y - extenty, camera->position.Y + extenty, 0, 1);
	camera->view = HMM_Mat4d(1);
}