#include <flight.hpp>

flight::flight() {

}
flight::~flight() {

}

void flight::update(physics &physics) {
	foreach([&](flight_entity &flight_entity) {
		physics.get(flight_entity.id, [&](physics_entity &physics_entity) {
			glm::vec2 thrust = flight_entity.thrust;
			thrust.y = glm::max(0.0f, thrust.y); // no reverse
			float speed2 = glm::length2(thrust);
			if (speed2 > 1) {
				thrust = glm::normalize(thrust);
			}
			thrust = glm::rotate(thrust, physics_entity.angle);
			
			glm::vec2 force = thrust * flight_entity.max_thrust;
			float turn = flight_entity.turn;
			turn = glm::clamp(turn, -1.0f, 1.0f);
			float torque = turn * flight_entity.max_turn;

			// linear damping ramps off at higher speeds
			//glm::vec2 force_damping = glm::mix(-control_entity.linear_damping * physics_entity.speed, glm::vec2(0), speed_percent);
			glm::vec2 force_damping = -flight_entity.linear_damping * physics_entity.velocity;
			force += force_damping;

			float torque_damping = -flight_entity.angular_damping * physics_entity.angular_velocity;
			torque += torque_damping;

			physics_entity.force = force;
			physics_entity.torque = torque;
		});
	});
}