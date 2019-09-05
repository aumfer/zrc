#include <draw_map.hpp>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
uniform mat4 projection;

layout(location = 0) in vec2 position;

out vec2 v_position;

void main() {
	mat4 inv_projection = inverse(projection);

	vec4 p = vec4(position, 0, 1);
	vec4 world_position = inv_projection * p;
	v_position = world_position.xy;
	gl_Position = p;
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
#include <shaders/sdf.glsl>
GLSL(
uniform ivec2 resolution;
uniform float map_scale;

layout(binding=0) uniform sampler2DArray lines;
layout(binding=1) uniform usampler2D counts;

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

out vec4 fragColor;

void main() {
	random_init2f(v_position);

	vec3 color = vec3(0);
	ivec2 map_coord = ivec2(v_position / map_scale);

	//uvec4 gcounts = textureGather(counts, vec2(map_coord) / textureSize(counts, 0));

	uint max_count = 0;
	/*
	const int RANGE = 0;
	uint sum = 0;
	for (int x = map_coord.x - RANGE; x <= map_coord.x + RANGE; ++x) {
		for (int y = map_coord.y - RANGE; y <= map_coord.y + RANGE; ++y) {
			ivec2 sample_coord = ivec2(x, y);
			if (sample_coord == map_coord) {
				continue;
			}
			uint count = texelFetch(counts, sample_coord, 0).r;
			sum += count;
			max_count = max(max_count, count);
			for (int i = 0; i < count; ++i) {
				vec4 line = texelFetch(lines, ivec3(sample_coord, i), 0);
				int instanceId = floatBitsToInt(line.x);

				instance_t instance = instances[instanceId];
				float distSq = 1e10;
				distSq = min(distSq, sdBezierSq(v_position, instance.point0, instance.point1, instance.point2));
				distSq = min(distSq, sdBezierSq(v_position, instance.point1, instance.point2, instance.point3));
				distSq = min(distSq, sdBezierSq(v_position, instance.point2, instance.point3, instance.point0));
				//float distSq = sdCircle(instance.position - v_position, length(instance.size)/2);
				//distSq *= distSq;
				//distSq = sqrt(distSq);
				distSq += 1;

				color += instance.color.rgb / distSq;
			}
		}
	}
	if (sum > 0) {
		//color /= vec3(sum);
	}
	color /= vec3(9);
	//color += vec3(float(sum) / 9.0);
	*/

	float d = 1e10;
	//d = min(d, grid_line((v_position + map_scale / 2) / map_scale) * map_scale);
	d = min(d, grid_point((v_position + map_scale / 2) / map_scale) * map_scale);
	//d = min(d, hexagon(v_position, map_scale).z);
	//d -= 1;
	//d = max(0, d);

	color += rgb(3, 3, 3);
	color += vec3(hash1(v_position)) * 0.01;
	color += rgb(100, 149, 237) * fill2(d) * 0.05;

	if (max_count > 6) {
		color = vec3(1, 0, 0);
	}

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
	uniforms.projection = glGetUniformLocation(program, "projection");
	uniforms.map_scale = glGetUniformLocation(program, "map_scale");
	glUniform1f(uniforms.map_scale, MAP_SCALE);
	glUseProgram(0);
}
draw_map::~draw_map() {

}

void draw_map::update(const ui &ui, const camera &camera, const curves_map &curves_map, const curves &curves) {
	glm::mat4 projection = camera.projection;

	glEnable(GL_FRAMEBUFFER_SRGB);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, curves_map.tile_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, curves_map.count_texture);

	glViewport(0, 0, ui.framebuffer_size.x, ui.framebuffer_size.y);
	glUseProgram(program);
	glUniform4ui(uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(uniforms.resolution, ui.framebuffer_size.x, ui.framebuffer_size.y);
	glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));
	
	fsq.begin();
	fsq.draw(1);
	fsq.end();

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	glDisable(GL_FRAMEBUFFER_SRGB);
}