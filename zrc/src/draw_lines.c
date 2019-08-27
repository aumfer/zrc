#include <draw_lines.h>
#include <HandmadeMath.h>

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

	vec4 p = vec4(position, 0, 1);
	v_position = p.xy;
	gl_Position = projection * p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
	uniform ivec2 resolution;

in vec2 v_position;
in flat vec4 v_color;

out vec4 fragColor;

void main() {
	random_init2f(v_position);

	vec3 color = v_color.rgb;

	fragColor = vec4(color, 1.0);
});

void draw_lines_create(draw_lines_t *draw_lines) {
	draw_lines->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, draw_lines->vertex_shader);
	lsgl_compileshader(draw_lines->vertex_shader, vertex_src, lsgl_countof(vertex_src));

	draw_lines->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, draw_lines->fragment_shader);
	lsgl_compileshader(draw_lines->fragment_shader, fragment_src, lsgl_countof(fragment_src));

	draw_lines->program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, draw_lines->program);
	glAttachShader(draw_lines->program, draw_lines->vertex_shader);
	glAttachShader(draw_lines->program, draw_lines->fragment_shader);
	lsgl_linkprogram(draw_lines->program);

	glUseProgram(draw_lines->program);
	draw_lines->uniforms.random_seed = glGetUniformLocation(draw_lines->program, "random_seed");
	draw_lines->uniforms.resolution = glGetUniformLocation(draw_lines->program, "resolution");
	draw_lines->uniforms.projection = glGetUniformLocation(draw_lines->program, "projection");
	glUseProgram(0);
}
void draw_lines_destroy(draw_lines_t *draw_lines) {

}

void draw_lines_update(draw_lines_t *draw_lines, lines_t *lines, const ui_t *ui, const camera_t *camera) {
	hmm_mat4 projection = camera->projection;
	//hmm_mat4 projection = HMM_Orthographic(0, MAP_SIZE, 0, MAP_SIZE, 0, 1);

	glViewport(0, 0, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUseProgram(draw_lines->program);
	glUniform4ui(draw_lines->uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(draw_lines->uniforms.resolution, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUniformMatrix4fv(draw_lines->uniforms.projection, 1, GL_FALSE, (const GLfloat *)&projection);

	for (int i = 0; i < 1; ++i) { // todo try rebind framebuffer to get > 8 outputs
		lines_begin(lines);
		lines_draw(lines, 1);
		lines_end(lines);
	}

	glUseProgram(0);
}