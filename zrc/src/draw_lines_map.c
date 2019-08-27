#include <draw_lines_map.h>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
	uniform mat4 projection;

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec2 v_position;
out flat vec4 v_color;

void main() {
	v_color = color;

	vec4 p = projection * vec4(position, 0, 1);
	v_position = p.xy;
	gl_Position = p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform ivec2 resolution;
layout(binding = 0, r32ui) uniform uimage2D count_texture;

in vec2 v_position;
in flat vec4 v_color;

layout(location = 0) out vec4 fragColor0;
layout(location = 1) out vec4 fragColor1;
layout(location = 2) out vec4 fragColor2;
layout(location = 3) out vec4 fragColor3;
layout(location = 4) out vec4 fragColor4;
layout(location = 5) out vec4 fragColor5;
layout(location = 6) out vec4 fragColor6;
layout(location = 7) out vec4 fragColor7;

void main() {
	random_init2f(v_position);

	ivec2 image_coord = ivec2(gl_FragCoord.xy);
	uint count = imageAtomicAdd(count_texture, image_coord, 1);

	switch (count) {
	case 0:
		fragColor0 = v_color;
		break;
	case 1:
		fragColor1 = v_color;
		break;
	case 2:
		fragColor2 = v_color;
		break;
	case 3:
		fragColor3 = v_color;
		break;
	case 4:
		fragColor4 = v_color;
		break;
	case 5:
		fragColor5 = v_color;
		break;
	case 6:
		fragColor6 = v_color;
		break;
	case 7:
		fragColor7 = v_color;
		break;
	}
});

void draw_lines_map_create(draw_lines_map_t *draw_lines_map) {
	draw_lines_map->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, draw_lines_map->vertex_shader);
	lsgl_compileshader(draw_lines_map->vertex_shader, vertex_src, lsgl_countof(vertex_src));

	draw_lines_map->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, draw_lines_map->fragment_shader);
	lsgl_compileshader(draw_lines_map->fragment_shader, fragment_src, lsgl_countof(fragment_src));

	draw_lines_map->program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, draw_lines_map->program);
	glAttachShader(draw_lines_map->program, draw_lines_map->vertex_shader);
	glAttachShader(draw_lines_map->program, draw_lines_map->fragment_shader);
	lsgl_linkprogram(draw_lines_map->program);

	glUseProgram(draw_lines_map->program);
	draw_lines_map->uniforms.random_seed = glGetUniformLocation(draw_lines_map->program, "random_seed");
	draw_lines_map->uniforms.resolution = glGetUniformLocation(draw_lines_map->program, "resolution");
	draw_lines_map->uniforms.projection = glGetUniformLocation(draw_lines_map->program, "projection");
	draw_lines_map->uniforms.count_texture = glGetUniformLocation(draw_lines_map->program, "count_texture");
	glUseProgram(0);
}
void draw_lines_map_destroy(draw_lines_map_t *draw_lines_map) {

}

void draw_lines_map_update(draw_lines_map_t *draw_lines_map, lines_t *lines, const lines_map_t *lines_map) {
	glClearTexImage(lines_map->count_texture, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	hmm_mat4 projection = HMM_Orthographic(0, WORLD_SIZE, 0, WORLD_SIZE, 0, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, lines_map->framebuffer);
	glBindTexture(GL_TEXTURE_2D, lines_map->count_texture);

	glViewport(0, 0, MAP_SIZE, MAP_SIZE);
	glUseProgram(draw_lines_map->program);
	glUniform4ui(draw_lines_map->uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(draw_lines_map->uniforms.resolution, MAP_SIZE, MAP_SIZE);
	glUniformMatrix4fv(draw_lines_map->uniforms.projection, 1, GL_FALSE, (const GLfloat *)&projection);

	lines_begin(lines);
	lines_draw(lines, 1);
	lines_end(lines);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}