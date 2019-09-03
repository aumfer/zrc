#include <draw_curves_map.h>
#include <glm/gtc/matrix_transform.hpp>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform mat4 projection;
uniform float map_scale;

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

	// force each curve to be at least MAP_SCALE large so it rasterizes
	vec2 raster_size = max(vec2(map_scale /2), instance.size);
	// nvm if NV_conservative_raster
	//raster_size = instance.size;
	vec2 raster_position = instance.position + (position * raster_size);
	v_position = instance.position + (position * instance.size);
	gl_Position = projection * vec4(raster_position, 0, 1);
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform ivec2 resolution;
uniform float map_scale;

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

layout(binding = 1, r32ui) uniform uimage2D count_texture;

in vec2 v_position;
in flat int v_instance;

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

	ivec2 image_coord = ivec2(v_position / map_scale);
	uint count = imageAtomicAdd(count_texture, image_coord, 1);

	if (count > 8) {
		discard;
	}

	instance_t instance = instances[v_instance];
	vec4 result = vec4(intBitsToFloat(v_instance), 0, 1, 1);

	switch (count) {
	case 0:
		fragColor0 = result;
		break;
	case 1:
		fragColor1 = result;
		break;
	case 2:
		fragColor2 = result;
		break;
	case 3:
		fragColor3 = result;
		break;
	case 4:
		fragColor4 = result;
		break;
	case 5:
		fragColor5 = result;
		break;
	case 6:
		fragColor6 = result;
		break;
	case 7:
		fragColor7 = result;
		break;
	}
});

draw_curves_map::draw_curves_map() {
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
	uniforms.count_texture = glGetUniformLocation(program, "count_texture");
	uniforms.map_scale = glGetUniformLocation(program, "map_scale");
	glUniform1f(uniforms.map_scale, MAP_SCALE);
	glUseProgram(0);
}
draw_curves_map::~draw_curves_map() {

}

void draw_curves_map::update(curves_t &curves, const curves_map_t &curves_map) {
	glClearTexImage(curves_map.count_texture, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glm::mat4 projection = glm::ortho(0, WORLD_SIZE, 0, WORLD_SIZE, 0, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, curves_map.framebuffer);
	glBindTexture(GL_TEXTURE_2D, curves_map.count_texture);
	glClear(GL_COLOR_BUFFER_BIT);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	//glBlendEquation(GL_FUNC_ADD);

	glViewport(0, 0, MAP_SIZE, MAP_SIZE);
	glUseProgram(program);
	glUniform4ui(uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(uniforms.resolution, MAP_SIZE, MAP_SIZE);
	glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL_RECTANGLE_NV);
	glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
	//glEnable(GL_DEPTH_CLAMP);
	curves.begin();
	curves.draw();
	curves.end();
	//glDisable(GL_DEPTH_CLAMP);
	glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(0);

	glDisable(GL_BLEND);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}