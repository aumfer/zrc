#include <curves_map.h>
#include <assert.h>

curves_map::curves_map() {
	glGenTextures(1, &count_texture);
	lsgl_object_label(GL_TEXTURE, count_texture);
	glBindTexture(GL_TEXTURE_2D, count_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, MAP_SIZE, MAP_SIZE);
	glBindImageTexture(CURVES_MAP_COUNT_IMAGE_BINDING, count_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &tile_texture);
	lsgl_object_label(GL_TEXTURE, tile_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, tile_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, MAP_SIZE, MAP_SIZE, CURVES_MAP_COUNT);

	glGenFramebuffers(1, &framebuffer);
	lsgl_object_label(GL_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLenum draw_buffers[CURVES_MAP_COUNT];
	for (int i = 0; i < CURVES_MAP_COUNT; ++i) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tile_texture, 0, i);
		draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(CURVES_MAP_COUNT, draw_buffers);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
curves_map::~curves_map() {
}

void curves_map::update(const visual &visual) {
}