#include <draw_map.hpp>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform mat4 inv_view_projection;

layout(location = 0) in vec2 position;

out vec2 v_position;

void main() {
	vec4 p = vec4(position, 0, 1);
	vec4 world_position = inv_view_projection * p;
	v_position = world_position.xy;
	gl_Position = p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
#include <shaders/sdf.glsl>
#include <shaders/curves.glsl>
GLSL(
uniform ivec2 resolution;
uniform float map_scale;

in vec2 v_position;

out vec4 fragColor;

void main() {
	random_init2f(v_position);

	vec3 color = vec3(0);
	ivec2 map_coord = ivec2(v_position / map_scale);

	float d = 1e10;
	d = min(d, grid_line((v_position) / map_scale) * map_scale);
	//d = min(d, grid_point((v_position + map_scale / 2) / map_scale) * map_scale);
	//d = min(d, hexagon(v_position, map_scale).z);

	color += rgb(3, 3, 3);
	color += vec3(hash1(v_position)) * 0.01;
	color += rgb(100, 149, 237) * fill2(d) * 0.05;

	fragColor = vec4(color, 1.0);
});

draw_map::draw_map() {
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, vertex_shader);
	lsgl_compileshader(vertex_shader, vertex_src, lsgl_countof(vertex_src));

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, fragment_shader);
	lsgl_compileshader(fragment_shader, fragment_src, lsgl_countof(fragment_src));

	program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, program);
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	lsgl_linkprogram(program);

	glUseProgram(program);
	uniforms.random_seed = glGetUniformLocation(program, "random_seed");
	uniforms.resolution = glGetUniformLocation(program, "resolution");
	uniforms.view_projection = glGetUniformLocation(program, "view_projection");
	uniforms.inv_view_projection = glGetUniformLocation(program, "inv_view_projection");
	uniforms.map_scale = glGetUniformLocation(program, "map_scale");
	glUniform1f(uniforms.map_scale, MAP_SCALE);
	glUseProgram(0);
}
draw_map::~draw_map() {

}

void draw_map::update(const ui &ui, const camera &camera, const curves &curves) {
	glm::mat4 view_projection = camera.view_projection;

	glEnable(GL_FRAMEBUFFER_SRGB);

	glViewport(0, 0, ui.framebuffer_size.x, ui.framebuffer_size.y);
	glUseProgram(program);
	glUniform4ui(uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(uniforms.resolution, ui.framebuffer_size.x, ui.framebuffer_size.y);
	glUniformMatrix4fv(uniforms.view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
	glUniformMatrix4fv(uniforms.inv_view_projection, 1, GL_FALSE, glm::value_ptr(glm::inverse(view_projection)));
	
	fsq.begin();
	fsq.draw(1);
	fsq.end();

	glUseProgram(0);

	glDisable(GL_FRAMEBUFFER_SRGB);
}