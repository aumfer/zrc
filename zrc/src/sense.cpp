#include <sense.hpp>

sense::sense() {

}
sense::~sense() {

}

void sense::update(const physics &physics) {
	foreach([&](sense_entity &sense_entity) {
		physics.get(sense_entity.id, [&](const physics_entity &physics_entity) {
			sense_entity.sensed.clear();
			physics.query_point(physics_entity.position, sense_entity.radius, [&](const physics_entity_t &sensed_physics) {
				sense_entity.sensed.add(sensed_physics.id);
			});
		});
	});
}