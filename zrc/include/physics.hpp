#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <chipmunk/chipmunk_private.h>
#include <zsys.hpp>
#include <lsm.hpp>
#include <zset.hpp>
#include <functional>

#define PHYSICS_MAX_ENTITIES 16384

typedef struct physics_entity {
	id_t id;

	cpBodyType type;
	cpBitmask collide_flags;
	cpBitmask collide_mask;
	cpBitmask response_mask;

	float radius;
	float max_speed;
	float max_spin;

	glm::vec2 position;
	float angle;
	glm::vec2 velocity;
	float angular_velocity;

	glm::vec2 force;
	float torque;

	cpBody *body;
	cpCircleShape *circle;

	zset contacts;
} physics_entity_t;

typedef struct physics : zsys<physics_entity, PHYSICS_MAX_ENTITIES> {
	cpSpace *space;
	cpCollisionHandler *collision_handler;

	physics();
	~physics();

	void add(const physics_entity &);
	void del(id);
	void update(float dt);

	physics_entity *query_ray(const glm::vec2 &start, const glm::vec2 &end, float radius) const;
	physics_entity *query_point(const glm::vec2 &p, float max_distance) const;

	typedef std::function<void(const physics_entity &, const glm::vec2 &)> ray_query_receiver;
	typedef std::function<void(const physics_entity &)> point_query_receiver;

	template<typename R>
	void query_ray(const glm::vec2 &start, const glm::vec2 &end, float radius, const R &receiver) const {
		ray_query_receiver func = ray_query_receiver(receiver);
		cpSpaceSegmentQuery(space, cpv(start.x, start.y), cpv(end.x, end.y), radius, CP_SHAPE_FILTER_ALL, [](cpShape *shape, cpVect point, cpVect normal, cpFloat alpha, void *data) {
			ray_query_receiver &receiver = *(ray_query_receiver *)data;
			const physics_entity *physics_entity = (const physics_entity_t *)cpShapeGetUserData(shape);
			glm::vec2 p = glm::vec2(float(point.x), float(point.y));
			receiver(*physics_entity, p);
		}, &func);
	}

	template<typename R>
	void query_point(const glm::vec2 &p, float max_distance, const R &receiver) const {
		point_query_receiver func = point_query_receiver(receiver);
		cpSpacePointQuery(space, cpv(p.x, p.y), max_distance, CP_SHAPE_FILTER_ALL, [](cpShape *shape, cpVect point, cpFloat distance, cpVect gradiant, void *data) {
			point_query_receiver &receiver = *(point_query_receiver *)data;
			const physics_entity *physics_entity = (const physics_entity_t *)cpShapeGetUserData(shape);
			receiver(*physics_entity);
		}, &func);
	}
} physics_t;

#endif