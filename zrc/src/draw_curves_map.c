#include <draw_curves_map.h>

static GLchar vertex_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
	uniform mat4 projection;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 world_position;
layout(location = 2) in vec2 size;
layout(location = 3) in vec4 color;

out vec2 v_position;
out flat vec4 v_color;
out flat int v_instance;

void main() {
	v_color = color;
	v_instance = gl_InstanceID;

	const float MAP_SCALE = 16; // todo
	// force each curve to be at least MAP_SCALE large so it rasterizes
	vec2 raster_size = max(vec2(MAP_SCALE), size);
	vec2 raster_position = world_position + (position * raster_size);
	v_position = world_position + (position * size);
	gl_Position = projection * vec4(raster_position, 0, 1);
});

static GLchar fragment_src[] = GLSL_BEGIN
#include <shaders/util.glsl>
GLSL(
	uniform ivec2 resolution;
layout(binding = 1, r32ui) uniform uimage2D count_texture;

in vec2 v_position;
in flat vec4 v_color;
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

	const float MAP_SCALE = 16; //todo

	//ivec2 image_coord = ivec2(gl_FragCoord.xy);
	ivec2 image_coord = ivec2(v_position / MAP_SCALE);
	uint count = imageAtomicAdd(count_texture, image_coord, 1);

	if (count > 8) {
		discard;
	}
	vec4 result = vec4(v_position, uintBitsToFloat(packUnorm4x8(v_color)), intBitsToFloat(v_instance));

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

void draw_curves_map_create(draw_curves_map_t *draw_curves_map) {
	draw_curves_map->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	lsgl_object_label(GL_SHADER, draw_curves_map->vertex_shader);
	lsgl_compileshader(draw_curves_map->vertex_shader, vertex_src, lsgl_countof(vertex_src));

	draw_curves_map->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	lsgl_object_label(GL_SHADER, draw_curves_map->fragment_shader);
	lsgl_compileshader(draw_curves_map->fragment_shader, fragment_src, lsgl_countof(fragment_src));

	draw_curves_map->program = glCreateProgram();
	lsgl_object_label(GL_PROGRAM, draw_curves_map->program);
	glAttachShader(draw_curves_map->program, draw_curves_map->vertex_shader);
	glAttachShader(draw_curves_map->program, draw_curves_map->fragment_shader);
	lsgl_linkprogram(draw_curves_map->program);

	glUseProgram(draw_curves_map->program);
	draw_curves_map->uniforms.random_seed = glGetUniformLocation(draw_curves_map->program, "random_seed");
	draw_curves_map->uniforms.resolution = glGetUniformLocation(draw_curves_map->program, "resolution");
	draw_curves_map->uniforms.projection = glGetUniformLocation(draw_curves_map->program, "projection");
	draw_curves_map->uniforms.count_texture = glGetUniformLocation(draw_curves_map->program, "count_texture");
	glUseProgram(0);
}
void draw_curves_map_destroy(draw_curves_map_t *draw_curves_map) {

}

void draw_curves_map_update(draw_curves_map_t *draw_curves_map, curves_t *curves, const curves_map_t *curves_map) {
	glClearTexImage(curves_map->count_texture, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	hmm_mat4 projection = HMM_Orthographic(0, WORLD_SIZE, 0, WORLD_SIZE, 0, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, curves_map->framebuffer);
	glBindTexture(GL_TEXTURE_2D, curves_map->count_texture);

	glViewport(0, 0, MAP_SIZE, MAP_SIZE);
	glUseProgram(draw_curves_map->program);
	glUniform4ui(draw_curves_map->uniforms.random_seed, rand(), rand(), rand(), rand());
	glUniform2i(draw_curves_map->uniforms.resolution, MAP_SIZE, MAP_SIZE);
	glUniformMatrix4fv(draw_curves_map->uniforms.projection, 1, GL_FALSE, (const GLfloat *)&projection);

	curves_begin(curves);
	curves_draw(curves);
	curves_end(curves);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}