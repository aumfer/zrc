#include <font.h>
#include <fonts/arial-16.h>
#include <fonts/consolas-16.h>
#include <assert.h>
#include <HandmadeMath.h>
#include <stdio.h>

typedef struct font_vertex {
	GLubvec4 color;
	GLusvec2 texcoord;
	GLvec2 position;
} font_vertex_t;
typedef GLuint font_index_t;

#define FONT_TEXTURE_UNIT 0

#define GLYPH_VERTICES 4
#define GLYPH_INDICES 6
#define MAX_GLYPHS (1024*128)

#define ARRAY_BUFFER_MAX ((MAX_GLYPHS)*(GLYPH_VERTICES))
#define ARRAY_BUFFER_SIZE (sizeof(font_vertex_t)*(ARRAY_BUFFER_MAX))

#define ELEMENT_ARRAY_BUFFER_MAX ((MAX_GLYPHS)*(GLYPH_INDICES))
#define ELEMENT_ARRAY_BUFFER_SIZE (sizeof(GLuint)*(ELEMENT_ARRAY_BUFFER_MAX))

static_assert(ARRAY_BUFFER_SIZE % 16 == 0, "array buffer size");
static_assert(ELEMENT_ARRAY_BUFFER_SIZE % 16 == 0, "element array buffer size");

static texture_font_t *font_data[] = {
	&font_arial16,
	&font_consolas16
};

static const GLchar vertex_source[] = GLSL_BEGIN GLSL(
uniform mat4 projection;

in vec4 color;
in vec2 texcoord;
in vec2 position;

out vec4 f_color;
out vec2 f_texcoord;

void main() {
	f_color = color;
	f_texcoord = texcoord;
	gl_Position = projection * vec4(position, 0, 1);
}
);
static const GLchar fragment_source[] = GLSL_BEGIN GLSL(
uniform sampler2D font;

in vec4 f_color;
in vec2 f_texcoord;

out vec4 p_color;

void main() {
	p_color = f_color * texture(font, f_texcoord).rrrr;
}
);

void font_create(font_t *font, font_style_t style) {
	font->style = style;
	font->data = font_data[style];

	glGenTextures(1, &font->texture);
	lsgl_object_label(GL_TEXTURE, font->texture);
	glBindTexture(GL_TEXTURE_2D, font->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, font->data->tex_width, font->data->tex_height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, font->data->tex_width, font->data->tex_height, GL_RED, GL_UNSIGNED_BYTE, font->data->tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &font->element_array_buffer);
	lsgl_object_label(GL_BUFFER, font->element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font->element_array_buffer);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, ELEMENT_ARRAY_BUFFER_SIZE, NULL, GL_MAP_WRITE_BIT);

	glGenBuffers(1, &font->array_buffer);
	lsgl_object_label(GL_BUFFER, font->array_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, font->array_buffer);
	glBufferStorage(GL_ARRAY_BUFFER, ARRAY_BUFFER_SIZE, NULL, GL_MAP_WRITE_BIT);

	font->shaders.vertex = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, font->shaders.vertex);
	lsgl_compileshader(font->shaders.vertex, vertex_source, lsgl_countof(vertex_source));
	font->shaders.fragment = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, font->shaders.fragment);
	lsgl_compileshader(font->shaders.fragment, fragment_source, lsgl_countof(fragment_source));
	font->program = glCreateProgram();
	glAttachShader(font->program, font->shaders.vertex);
	glAttachShader(font->program, font->shaders.fragment);
	lsgl_linkprogram(font->program);

	glUseProgram(font->program);
	lsgl_object_label(GL_PROGRAM, font->program);
	font->uniforms.projection = glGetUniformLocation(font->program, "projection");
	font->uniforms.font = glGetUniformLocation(font->program, "font");
	font->attributes.color = glGetAttribLocation(font->program, "color");
	font->attributes.texcoord = glGetAttribLocation(font->program, "texcoord");
	font->attributes.position = glGetAttribLocation(font->program, "position");
	glUniform1i(font->uniforms.font, FONT_TEXTURE_UNIT);

	glGenVertexArrays(1, &font->vertex_array);
	lsgl_object_label(GL_VERTEX_ARRAY, font->vertex_array);
	glBindVertexArray(font->vertex_array);

	glVertexAttribPointer(font->attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(font_vertex_t), lsgl_offsetof(font_vertex_t, color));
	glEnableVertexAttribArray(font->attributes.color);
	glVertexAttribPointer(font->attributes.texcoord, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(font_vertex_t), lsgl_offsetof(font_vertex_t, texcoord));
	glEnableVertexAttribArray(font->attributes.texcoord);
	glVertexAttribPointer(font->attributes.position, 2, GL_FLOAT, GL_FALSE, sizeof(font_vertex_t), lsgl_offsetof(font_vertex_t, position));
	glEnableVertexAttribArray(font->attributes.position);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}
void font_destroy(font_t *font) {
}

void font_begin(font_t *font) {
	font->glyph_count = 0;
}

void font_print(font_t *font, const char *text, GLvec2 position, GLubvec4 color) {
	size_t print_count;
	font_vertex_t *vertex;
	font_index_t *index;
	float pen[2];
	float x[2], y[2];
	size_t i;

	pen[0] = position[0];
	pen[1] = position[1];

	print_count = strnlen_s(text, MAX_GLYPHS - font->glyph_count);
	assert(font->glyph_count + print_count < MAX_GLYPHS);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font->element_array_buffer);
	index = (font_index_t *)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, font->glyph_count*GLYPH_INDICES * sizeof(font_index_t), print_count*GLYPH_INDICES * sizeof(font_index_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	assert(index);

	glBindBuffer(GL_ARRAY_BUFFER, font->array_buffer);
	vertex = (font_vertex_t *)glMapBufferRange(GL_ARRAY_BUFFER, font->glyph_count*GLYPH_VERTICES * sizeof(font_vertex_t), print_count*GLYPH_VERTICES * sizeof(font_vertex_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	assert(vertex);

	for (i = 0; i < print_count; ++i) {
		const texture_glyph_t *glyph = NULL;
		const texture_glyph_t *g;

		for (g = font->data->glyphs; g < font->data->glyphs + font->data->glyphs_count; ++g) {
			if (g->charcode == text[i]) {
				glyph = g;
				break;
			}
		}
		if (!glyph) {
			printf("font missing glyph %c", text[i]);
			continue;
		}

		if (i > 0) {
			const kerning_t *k;
			for (k = glyph->kerning; k < glyph->kerning + glyph->kerning_count; ++k) {
				if (k->charcode == text[i - 1]) {
					pen[0] += k->kerning;
				}
			}
		}

		x[0] = pen[0] + glyph->offset_x;
		y[0] = pen[1] + glyph->offset_y;
		x[1] = x[0] + glyph->width;
		y[1] = y[0] - glyph->height;

		vertex[0].color[0] = color[0];
		vertex[0].color[1] = color[1];
		vertex[0].color[2] = color[2];
		vertex[0].color[3] = color[3];
		vertex[0].texcoord[0] = (GLushort)(glyph->s0 * USHRT_MAX);
		vertex[0].texcoord[1] = (GLushort)(glyph->t0 * USHRT_MAX);
		vertex[0].position[0] = x[0];
		vertex[0].position[1] = y[0];

		vertex[1].color[0] = color[0];
		vertex[1].color[1] = color[1];
		vertex[1].color[2] = color[2];
		vertex[1].color[3] = color[3];
		vertex[1].texcoord[0] = (GLushort)(glyph->s0 * USHRT_MAX);
		vertex[1].texcoord[1] = (GLushort)(glyph->t1 * USHRT_MAX);
		vertex[1].position[0] = x[0];
		vertex[1].position[1] = y[1];

		vertex[2].color[0] = color[0];
		vertex[2].color[1] = color[1];
		vertex[2].color[2] = color[2];
		vertex[2].color[3] = color[3];
		vertex[2].texcoord[0] = (GLushort)(glyph->s1 * USHRT_MAX);
		vertex[2].texcoord[1] = (GLushort)(glyph->t1 * USHRT_MAX);
		vertex[2].position[0] = x[1];
		vertex[2].position[1] = y[1];

		vertex[3].color[0] = color[0];
		vertex[3].color[1] = color[1];
		vertex[3].color[2] = color[2];
		vertex[3].color[3] = color[3];
		vertex[3].texcoord[0] = (GLushort)(glyph->s1 * USHRT_MAX);
		vertex[3].texcoord[1] = (GLushort)(glyph->t0 * USHRT_MAX);
		vertex[3].position[0] = x[1];
		vertex[3].position[1] = y[0];

		index[0] = font->glyph_count * GLYPH_VERTICES + 0;
		index[1] = font->glyph_count * GLYPH_VERTICES + 1;
		index[2] = font->glyph_count * GLYPH_VERTICES + 2;
		index[3] = font->glyph_count * GLYPH_VERTICES + 0;
		index[4] = font->glyph_count * GLYPH_VERTICES + 2;
		index[5] = font->glyph_count * GLYPH_VERTICES + 3;

		pen[0] += glyph->advance_x;
		pen[1] += glyph->advance_y;

		vertex += GLYPH_VERTICES;
		index += GLYPH_INDICES;
		++font->glyph_count;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void font_draw(font_t *font, const ui_t *ui) {
	hmm_mat4 projection = HMM_Orthographic(0, (float)ui->framebuffer_size.x, 0, (float)ui->framebuffer_size.y, 0, 1);

	glViewport(0, 0, ui->framebuffer_size.x, ui->framebuffer_size.y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0 + FONT_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, font->texture);

	glUseProgram(font->program);
	glUniformMatrix4fv(font->uniforms.projection, 1, GL_FALSE, (const GLfloat *)&projection);

	glBindVertexArray(font->vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font->element_array_buffer);

	glDrawElements(GL_TRIANGLES, font->glyph_count*GLYPH_INDICES, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void font_end(font_t *font) {
}