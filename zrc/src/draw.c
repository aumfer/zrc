#include <draw.h>

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
GLSL(
uniform ivec2 resolution;
layout(binding=0) uniform sampler2DArray lines;
layout(binding=1) uniform usampler2D counts;

struct instance_t {
	vec2 position;
	vec2 size;
	vec2 point0;
	vec2 point1;
	vec2 point2;
	vec2 point3;
	int _color;
	ivec3 _;
};

layout(std430, binding = 0) buffer instance_buffer
{
	instance_t instances[];
};

in vec2 v_position;

out vec4 fragColor;

void main() {
	random_init2f(v_position);

	const float MAP_SCALE = 16; //todo

	vec3 color = vec3(0);
	ivec2 map_coord = ivec2(v_position / MAP_SCALE);
	//textureGather
	const int RANGE = 1;
	uint sum = 0;
	uint max_count = 0;
	for (int x = map_coord.x - RANGE; x <= map_coord.x + RANGE; ++x) {
		for (int y = map_coord.y - RANGE; y <= map_coord.y + RANGE; ++y) {
			ivec2 sample_coord = ivec2(x, y);
			uint count = texelFetch(counts, sample_coord, 0).r;
			sum += count;
			max_count = max(max_count, count);
			for (int i = 0; i < count; ++i) {
				vec4 line = texelFetch(lines, ivec3(sample_coord, i), 0);
				vec2 line_origin = line.xy;
				vec4 line_color = unpackUnorm4x8(floatBitsToUint(line.z));
				int instanceId = floatBitsToInt(line.w);
				float line_dist = distance(line_origin, v_position);
				//color += line_color.rgb * 0.1;
				//color += line_color.rgb / (1+line_dist*line_dist);

				instance_t instance = instances[instanceId];
				//float distSq = udBezierSq(instance.point0, instance.point1, instance.point2, instance.point3, v_position).x;
				//float distSq = sdSegmentSq(instance.point1, instance.point2, v_position);
				//float distSq = sdLine(v_position, instance.point0, instance.point1, 1);
				//float distSq = distance(instance.position, v_position);
				float distSq = line_dist;
				distSq *= distSq;

				color += line_color.rgb / distSq;
			}
		}
	}
	if (sum > 0) {
		//color /= vec3(sum);
	}
	//color += vec3(float(sum) / 9.0);

	float d = grid_line((v_position + MAP_SCALE / 2) / MAP_SCALE);
	//float d = hexagon(v_position, 1).z;
	//d -= 1;
	//d = max(0, d);

	color += rgb(25, 25, 25);
	color += vec3(hash1(v_position)) * 0.05;
	if (d < 0.00015) {
		color += rgb(100, 149, 237) / 8;
	}

	if (max_count > 6) {
		color = vec3(1, 0, 0);
	}

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
	draw->uniforms.projection = glGetUniformLocation(draw->program, "projection");
	glUseProgram(0);
}
void draw_destroy(draw_t *draw) {

}

void draw_update(draw_t *draw, const ui_t *ui, const camera_t *camera, const curves_map_t *curves_map, const curves_t *curves) {
	hmm_mat4 projection = camera->projection;

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, DRAW_INSTANCE_BUFFER_BINDING, curves->instance_buffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, curves_map->tile_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, curves_map->count_texture);

	glViewport(0, 0, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUseProgram(draw->program);
	glUniform4ui(draw->uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(draw->uniforms.resolution, ui->framebuffer_size.x, ui->framebuffer_size.y);
	glUniformMatrix4fv(draw->uniforms.projection, 1, GL_FALSE, (const GLfloat *)&projection);
	
	fsq_begin(&draw->fsq);
	fsq_draw(&draw->fsq, 1);
	fsq_end(&draw->fsq);

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}