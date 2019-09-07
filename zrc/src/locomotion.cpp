#include <locomotion.hpp>

locomotion::locomotion() {

}
locomotion::~locomotion() {

}

void locomotion::update(const physics &physics, flight &flight, float dt) {
	foreach([&](locomotion_entity &locomotion_entity) {
		if (!locomotion_entity.behavior) {
			return;
		}
		flight.get(locomotion_entity.id, [&](flight_entity &flight_entity) {
			physics.get(locomotion_entity.id, [&](const physics_entity &physics_entity) {
				const float THRUSTS[] = {
					0,
					1
				};
				const int NUM_THRUSTS = _countof(THRUSTS);
				const float TURNS[] = {
					-1,
					-0.5,
					0,
					+0.5,
					+1
				};
				const int NUM_TURNS = _countof(TURNS);

				glm::vec2 positions[NUM_THRUSTS][NUM_TURNS];
				float angles[NUM_THRUSTS][NUM_TURNS];

				double potentials[NUM_THRUSTS][NUM_TURNS] = {};
				for (int i = 0; i < NUM_THRUSTS; ++i) {
					for (int j = 0; j < NUM_TURNS; ++j) {
						float turn = flight_entity.max_turn * TURNS[j] * dt;
						float angle = physics_entity.angle + turn;
						float move = flight_entity.max_thrust * THRUSTS[i] * dt;
						glm::vec2 position = physics_entity.position + glm::rotate(glm::vec2(0, move), angle);

						positions[i][j] = position;
						angles[i][j] = angle;

						double potential = locomotion_entity.behavior(position, angle);
						potentials[i][j] = potential;
					}
				}

				int best_thrust = -1;
				int best_turn = -1;
				double best_potential = -FLT_MAX;
				for (int i = 0; i < NUM_THRUSTS; ++i) {
					for (int j = 0; j < NUM_TURNS; ++j) {
						double potential = potentials[i][j];
						if (potential >= best_potential) {
							best_potential = potential;
							best_thrust = i;
							best_turn = j;
						}
					}
				}

				flight_entity.thrust = glm::vec2(0, THRUSTS[best_thrust]);
				flight_entity.turn = TURNS[best_turn];
			});
		});
	});
}