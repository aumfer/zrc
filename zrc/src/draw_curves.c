#include <draw_curves.h>
#include <HandmadeMath.h>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform mat4 projection;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 world_position;
layout(location = 2) in vec2 size;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 point0;
layout(location = 5) in vec2 point1;
layout(location = 6) in vec2 point2;
layout(location = 7) in vec2 point3;

out vec2 v_position;
out flat vec4 v_color;
out flat vec2 v_point0;
out flat vec2 v_point1;
out flat vec2 v_point2;
out flat vec2 v_point3;

void main() {
	v_color = color;
	v_point0 = point0;
	v_point1 = point1;
	v_point2 = point2;
	v_point3 = point3;

	mat4 transform = mat4_translate(world_position.x, world_position.y, 0) * mat4_scale(size.x, size.y, 0);
	vec4 p = transform * vec4(position, 0, 1);
	v_position = p.xy;
	gl_Position = projection * p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform ivec2 resolution;

in vec2 v_position;
in flat vec4 v_color;
in flat vec2 v_point0;
in flat vec2 v_point1;
in flat vec2 v_point2;
in flat vec2 v_point3;

out vec4 fragColor;

void main() {
	float d = udBezierSq(v_point0, v_point1, v_point2, v_point3, v_position).x;
	//d += 1;
	//d *= 10;

	//float d = 1e10;
	//d = min(d, sdSegmentSq(v_position, v_point0, v_point1));
	//d = min(d, sdSegmentSq(v_position, v_point1, v_point2));
	//d = min(d, sdSegmentSq(v_position, v_point2, v_point3));
	//d = min(d, sdSegmentSq(v_position, v_point3, v_point0));
	//d = sqrt(d);

	vec3 color = v_color.rgb / d;

	fragColor = vec4(color, 1.0);
});

void draw_curves_create(draw_curves_t *draw_curves) {
	draw_curves->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, draw_curves->vertex_shader);
	lsgl_compileshader(draw_curves->vertex_shader, vertex_src, lsgl_countof(vertex_src));

	draw_curves->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, draw_curves->fragment_shader);
	lsgl_compileshader(draw_curves->fragment_shader, fragment_src, lsgl_countof(fragment_src));

	draw_curves->program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, draw_curves->program);
	glAttachShader(draw_curves->program, draw_curves->vertex_shader);
	glAttachShader(draw_curves->program, draw_curves->fragment_shader);
	lsgl_linkprogram(draw_curves->program);

	glUseProgram(draw_curves->program);
	draw_curves->uniforms.random_seed = glGetUniformLocation(draw_curves->program, "random_seed");
	draw_curves->uniforms.resolution = glGetUniformLocation(draw_curves->program, "resolution");
	draw_curves->uniforms.projection = glGetUniformLocation(draw_curves->program, "projection");
	glUseProgram(0);
}
void draw_curves_destroy(draw_curves_t *draw_curves) {
}

void draw_curves_update(draw_curves_t *draw_curves, curves_t *curves, const ui_t *ui, const camera_t *camera) {
	//hmm_mat4 projection = HMM_Orthographic(MAP_SIZE/4, MAP_SIZE/2, MAP_SIZE/4, MAP_SIZE/2, 0, 1);
	//hmm_mat4 projection = HMM_Orthographic(0, (float)ui->framebuffer_size.x, 0, (float)ui->framebuffer_size.y, 0, 1);
	//hmm_mat4 projection = HMM_Perspective(90, ui->framebuffer_aspect, 0.1, 10000);
	//hmm_mat4 view = HMM_LookAt(HMM_Vec3(MAP_SIZE/4, MAP_SIZE/4, 100), HMM_Vec3(MAP_SIZE/2, MAP_SIZE/2, 0), HMM_Vec3(0, 0, 1));
	//projection = HMM_MultiplyMat4(projection, view);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glViewport(0, 0, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUseProgram(draw_curves->program);
	glUniform4ui(draw_curves->uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(draw_curves->uniforms.resolution, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUniformMatrix4fv(draw_curves->uniforms.projection, 1, GL_FALSE, (const GLfloat *)&camera->projection);

	curves_begin(curves);
	curves_draw(curves);
	curves_end(curves);

	glUseProgram(0);

	glDisable(GL_BLEND);
}