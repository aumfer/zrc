#include <lines_map.h>
#include <assert.h>

void lines_map_create(lines_map_t *lines_map) {
	glGenTextures(1, &lines_map->count_texture);
	lsgl_object_label(GL_TEXTURE, lines_map->count_texture);
	glBindTexture(GL_TEXTURE_2D, lines_map->count_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, MAP_SIZE, MAP_SIZE);
	glBindImageTexture(COUNT_IMAGE_BINDING, lines_map->count_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &lines_map->tile_texture);
	lsgl_object_label(GL_TEXTURE, lines_map->tile_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lines_map->tile_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, MAP_SIZE, MAP_SIZE, LINES_MAP_COUNT);

	glGenFramebuffers(1, &lines_map->framebuffer);
	lsgl_object_label(GL_FRAMEBUFFER, lines_map->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, lines_map->framebuffer);

	GLenum draw_buffers[LINES_MAP_COUNT];
	for (int i = 0; i < LINES_MAP_COUNT; ++i) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, lines_map->tile_texture, 0, i);
		draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(LINES_MAP_COUNT, draw_buffers);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void lines_map_destroy(lines_map_t *lines_map) {

}

void lines_map_update(lines_map_t *lines_map, lines_t *lines) {

}