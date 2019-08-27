#include <draw.h>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
layout(location = 0) in vec2 position;

out vec2 v_position;

void main() {
	vec4 p = vec4(position, 0, 1);
	v_position = p.xy;
	gl_Position = p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform ivec2 resolution;

in vec2 v_position;

out vec4 fragColor;

void main() {
	random_init2f(v_position);

	float d = grid_line(v_position);

	//vec3 color = vec3(100 / 255.0, 149 / 255.0, 237 / 255.0);
	vec3 color = vec3(25 / 255.0, 25 / 255.0, 25 / 255.0) + vec3(hash1(v_position*100)) * 0.05;
	//color /= d;

	fragColor = vec4(color, 1.0);
});

void draw_create(draw_t *draw) {
	fsq_create(&draw->fsq);

	draw->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, draw->vertex_shader);
	lsgl_compileshader(draw->vertex_shader, vertex_src, lsgl_countof(vertex_src));

	draw->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, draw->fragment_shader);
	lsgl_compileshader(draw->fragment_shader, fragment_src, lsgl_countof(fragment_src));

	draw->program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, draw->program);
	glAttachShader(draw->program, draw->vertex_shader);
	glAttachShader(draw->program, draw->fragment_shader);
	lsgl_linkprogram(draw->program);

	glUseProgram(draw->program);
	draw->uniforms.random_seed = glGetUniformLocation(draw->program, "random_seed");
	draw->uniforms.resolution = glGetUniformLocation(draw->program, "resolution");
	glUseProgram(0);
}
void draw_destroy(draw_t *draw) {

}

void draw_update(draw_t *draw, const ui_t *ui) {
	glViewport(0, 0, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUseProgram(draw->program);
	glUniform4ui(draw->uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(draw->uniforms.resolution, ui->framebuffer_size.x, ui->framebuffer_size.y);
	
	fsq_begin(&draw->fsq);
	fsq_draw(&draw->fsq, 1);
	fsq_end(&draw->fsq);

	glUseProgram(0);
}