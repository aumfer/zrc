#include <draw_curves.h>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform mat4 projection;

struct instance_t {
	vec2 position;
	vec2 size;
	vec4 color;
	vec2 point0;
	vec2 point1;
	vec2 point2;
	vec2 point3;
};

layout(std430, binding = 0) buffer instance_buffer
{
	instance_t instances[];
};

layout(location = 0) in vec2 position;

out vec2 v_position;
out flat int v_instance;

void main() {
	instance_t instance = instances[gl_InstanceID];
	v_instance = gl_InstanceID;

	mat4 transform = mat4_translate(instance.position.x, instance.position.y, 0) * mat4_scale(instance.size.x, instance.size.y, 0);
	vec4 p = transform * vec4(position, 0, 1);
	v_position = p.xy;
	gl_Position = projection * p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
#include <shaders/sdf.glsl>
GLSL(
uniform ivec2 resolution;

struct instance_t {
	vec2 position;
	vec2 size;
	vec4 color;
	vec2 point0;
	vec2 point1;
	vec2 point2;
	vec2 point3;
};

layout(std430, binding = 0) buffer instance_buffer
{
	instance_t instances[];
};

in vec2 v_position;
in flat int v_instance;

out vec4 fragColor;

void main() {
	instance_t instance = instances[v_instance];

	float d = 1e10;
	d = min(d, sdBezierSq(v_position, instance.point0, instance.point1, instance.point2));
	d = min(d, sdBezierSq(v_position, instance.point1, instance.point2, instance.point3));
	d = min(d, sdBezierSq(v_position, instance.point2, instance.point3, instance.point0));
	d = min(d, sdBezierSq(v_position, instance.point3, instance.point0, instance.point1));

	//float d = 1e10;
	//d = min(d, sdSegmentSq(v_position, instance.point0, instance.point1));
	//d = min(d, sdSegmentSq(v_position, instance.point1, instance.point2));
	//d = min(d, sdSegmentSq(v_position, instance.point2, instance.point3));
	//d = min(d, sdSegmentSq(v_position, instance.point3, instance.point0));
	//d = sqrt(d);

	//float d = 1e10;
	//d = min(d, udBezierSq(instance.point0, instance.point1, instance.point2, instance.point3, v_position).x);
	//d = min(d, udBezierSq(instance.point1, instance.point2, instance.point3, instance.point0, v_position).x);
	//d = min(d, udBezierSq(instance.point2, instance.point3, instance.point0, instance.point1, v_position).x);
	//d = min(d, udBezierSq(instance.point3, instance.point0, instance.point1, instance.point2, v_position).x);

	//d = max(1, d);
	//d += 1;
	d *= 2;

	vec3 color = instance.color.rgb / d;

	fragColor = vec4(color, 1);
});

draw_curves::draw_curves() {
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
	uniforms.projection = glGetUniformLocation(program, "projection");
	glUseProgram(0);
}
draw_curves::~draw_curves() {
}

void draw_curves::update(curves &curves, const ui &ui, const camera &camera) {
	glm::mat4 projection = camera.projection;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glViewport(0, 0, ui.framebuffer_size.x, ui.framebuffer_size.y);
	glUseProgram(program);
	glUniform4ui(uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(uniforms.resolution, ui.framebuffer_size.x, ui.framebuffer_size.y);
	glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));

	curves.begin();
	curves.draw();
	curves.end();

	glUseProgram(0);

	glDisable(GL_BLEND);
}