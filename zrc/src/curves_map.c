#include <curves_map.h>
#include <assert.h>

void curves_map_create(curves_map_t *curves_map) {
	glGenTextures(1, &curves_map->count_texture);
	lsgl_object_label(GL_TEXTURE, curves_map->count_texture);
	glBindTexture(GL_TEXTURE_2D, curves_map->count_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, MAP_SIZE, MAP_SIZE);
	glBindImageTexture(CURVES_MAP_COUNT_IMAGE_BINDING, curves_map->count_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &curves_map->tile_texture);
	lsgl_object_label(GL_TEXTURE, curves_map->tile_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, curves_map->tile_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, MAP_SIZE, MAP_SIZE, CURVES_MAP_COUNT);

	glGenFramebuffers(1, &curves_map->framebuffer);
	lsgl_object_label(GL_FRAMEBUFFER, curves_map->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, curves_map->framebuffer);

	GLenum draw_buffers[CURVES_MAP_COUNT];
	for (int i = 0; i < CURVES_MAP_COUNT; ++i) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, curves_map->tile_texture, 0, i);
		draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(CURVES_MAP_COUNT, draw_buffers);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void curves_map_destroy(curves_map_t *curves_map) {
}

void curves_map_update(curves_map_t *curves_map, const visual_t *visual) {
}