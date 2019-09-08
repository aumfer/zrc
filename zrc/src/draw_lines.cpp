#include <draw_lines.hpp>

draw_lines::draw_lines() {
}
draw_lines::~draw_lines() {

}

void draw_lines::update(const ui &ui, const camera &camera, const visual &visual) {
	if (!vector_display) {
		vector_display_new(&vector_display, ui.framebuffer_size.x, ui.framebuffer_size.y);
		vector_display_setup(vector_display);
	} else {
		double w, h;
		vector_display_get_size(vector_display, &w, &h);
		if (w != ui.framebuffer_size.x || h != ui.framebuffer_size.y) {
			vector_display_resize(vector_display, ui.framebuffer_size.x, ui.framebuffer_size.y);
		}
	}

	double scale = 1.0 / camera.zoom;
	vector_display_set_transform(vector_display, -camera.position.x * scale, -camera.position.y * scale, scale);

	visual.foreach([&](const visual_entity &visual_entity) {
		glm::vec4 p = visual_entity.transform * glm::vec4(visual_entity.points[0], 0, 1);
		vector_display_begin_draw(vector_display, p.x, p.y);
		for (int i = 1; i < VISUAL_ENTITY_POINTS; ++i) {
			p = visual_entity.transform * glm::vec4(visual_entity.points[i], 0, 1);
			vector_display_draw_to(vector_display, p.x, p.y);
		}
		vector_display_end_draw(vector_display);
	});

	vector_display_update(vector_display);
}