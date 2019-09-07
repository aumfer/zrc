#include <seek.hpp>

seek::seek() {

}
seek::~seek() {

}

void seek::update(const physics &physics, locomotion &locomotion) {
	foreach([&](seek_entity &seek_entity) {
		locomotion.get(seek_entity.id, [&](locomotion_entity &locomotion_entity) {
			glm::vec2 to = seek_entity.to;
			locomotion_entity.behavior = [=](const glm::vec2 &sample_point, float angle) {
				float sample_distance = glm::distance(to, sample_point);
				float potential = WORLD_SIZE - glm::clamp(sample_distance, 0.0f, float(WORLD_SIZE));
				potential /= WORLD_SIZE;
				assert(!isnan(potential));
				return potential;
			};
		});
	});
}