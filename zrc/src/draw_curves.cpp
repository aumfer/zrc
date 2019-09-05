#include <draw_curves.hpp>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
#include <shaders/curves.glsl>
GLSL(
uniform mat4 projection;

layout(location = 0) in vec2 position;

out vec2 v_position;
out flat int v_instance;

void main() {
	instance_t instance = instances[gl_InstanceID];
	v_instance = gl_InstanceID;

	mat4 transform = instance.transform;
	vec4 p = transform * vec4(position, 0, 1);
	v_position = p.xy;
	gl_Position = projection * p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
#include <shaders/sdf.glsl>
#include <shaders/curves.glsl>
GLSL(
	uniform ivec2 resolution;

in vec2 v_position;
in flat int v_instance;

instance_t instance;

out vec4 fragColor;

float curve() {
	float d = 1e10;

	//d = min(d, sdBezierSq(v_position, instance.points[0], instance.points[1], instance.points[2]));

	//d = min(d, sdSegmentSq(v_position, instance.points[0], instance.points[1]));

	//d = min(d, udBezierSq(v_position, instance.points[0], instance.points[1], instance.points[2], instance.points[3]).x);

	vec2 p = instance.points[0];
	for (int i = 0; i < INSTANCE_POINTS - 3; ++i) {
		vec2 A = instance.points[i + 0];
		vec2 B = instance.points[i + 1];
		vec2 C = instance.points[i + 2];
		vec2 D = instance.points[i + 3];

		for (float t = 0.0; t <= 1.01; t += 0.1) {
			vec2 q = interpolate(A, B, C, D, t);

			d = min(d, sdSegmentSq(v_position, p, q));
			p = q;
		}
	}

	return d;
}

void main() {
	instance = instances[v_instance];

	float d = curve();

	float circle = sdCircle(v_position - instance.position, instance.radius);

	//d = max(1, d);
	//d += 1;
	//d *= 2;
	//d = clamp(d* (.75 + sin(gl_FragCoord.x*M_PI*.5)*.5), 0.0, 1.0);

	if (circle > 0) {
		discard;
	}

	d = min(d, abs(circle) - 0.1);

	vec4 color = unpackUnorm4x8(instance.color);
	color *= fill2(d);
	//color /= (d*d);
	//color += clamp(color / d, 0, 1) * 0.5;
	//color *= 1 - hash1(v_position) * 0.5;

	if ((instance.flags & INSTANCE_FLAGS_SELECTED) != 0) {
		color += vec4(rgb(100, 149, 237), 0) / (circle*circle);
	}

	fragColor = vec4(color.rgb, 1);
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

	glEnable(GL_FRAMEBUFFER_SRGB);

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

	glDisable(GL_FRAMEBUFFER_SRGB);
}