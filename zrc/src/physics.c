#include <physics.h>
#include <assert.h>

static void physics_store(physics_entity_t *);
static void physics_load(physics_entity_t *);
static void physics_velocity_func(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
static cpBool physics_collision_begin_func(cpArbiter *arb, cpSpace *space, cpDataPointer userData);
static void physics_collision_separate_func(cpArbiter *arb, cpSpace *space, cpDataPointer userData);

void physics_create(physics_t *physics) {
	kh_resize_fit(physics_map, &physics->map, PHYSICS_MAX_ENTITIES);

	cpSpaceInit(&physics->space);
	cpSpaceSetUserData(&physics->space, physics);

	physics->collision_handler = cpSpaceAddDefaultCollisionHandler(&physics->space);

	physics->collision_handler->beginFunc = physics_collision_begin_func;
	physics->collision_handler->separateFunc = physics_collision_separate_func;
}
void physics_destroy(physics_t *physics) {
}

void physics_add(physics_t *physics, physics_entity_t *physics_entity) {
	assert(kh_size(&physics->map) < PHYSICS_MAX_ENTITIES);

	//cpFloat mass = physics_entity->radius * physics_entity->radius;
	cpFloat mass = 1;
	cpFloat moment = cpMomentForCircle(mass, 0, physics_entity->radius*2, cpvzero);
	physics_entity->body = cpBodyAlloc();
	cpBodyInit(physics_entity->body, mass, moment);
	cpBodySetType(physics_entity->body, physics_entity->type);
	physics_store(physics_entity);
	cpBodySetVelocityUpdateFunc(physics_entity->body, physics_velocity_func);

	physics_entity->circle = cpCircleShapeAlloc();
	cpVect offset = cpvzero;
	//cpVect offset = cpv(physics_entity->radius, physics_entity->radius);
	cpCircleShapeInit(physics_entity->circle, physics_entity->body, physics_entity->radius, offset);
	cpShapeFilter filter;
	filter.group = id_hash_func(physics_entity->id);
	filter.categories = physics_entity->collide_flags;
	filter.mask = physics_entity->collide_mask;
	cpShapeSetFilter(&physics_entity->circle->shape, filter);
	//cpShapeSetElasticity(&e.circle.shape, 0.0f);
	//cpShapeSetFriction(&e.circle.shape, 0.7f);

	int absent;
	khint_t k = kh_put(physics_map, &physics->map, physics_entity->id, &absent);
	kh_val(&physics->map, k) = *physics_entity;
	physics_entity = &kh_val(&physics->map, k);
	cpBodySetUserData(physics_entity->body, physics_entity);
	cpShapeSetUserData(&physics_entity->circle->shape, physics_entity);
	cpSpaceAddBody(&physics->space, physics_entity->body);
	cpSpaceAddShape(&physics->space, &physics_entity->circle->shape);
}
void physics_remove(physics_t *physics, id_t id) {
	khint_t k = kh_get(physics_map, &physics->map, id);
	assert(k != kh_end(&physics->map));
	
	physics_entity_t physics_entity = kh_val(&physics->map, k);
	kh_del(physics_map, &physics->map, k);
	cpSpaceRemoveShape(&physics->space, &physics_entity.circle->shape);
	cpSpaceRemoveBody(&physics->space, physics_entity.body);
	cpShapeFree(&physics_entity.circle->shape);
	cpBodyFree(physics_entity.body);
}
physics_entity_t *physics_get(physics_t *physics, id_t id) {
	khint_t k = kh_get(physics_map, &physics->map, id);
	assert(k != kh_end(&physics->map));
	return &kh_val(&physics->map, k);
}

void physics_update(physics_t *physics, float dt) {
	physics_entity_t *physics_entity;
	kh_foreach_value_ptr(&physics->map, physics_entity, {
		physics_store(physics_entity);
	});
	cpSpaceStep(&physics->space, dt);
	kh_foreach_value_ptr(&physics->map, physics_entity, {
		physics_load(physics_entity);
	});
}

static void physics_store(physics_entity_t *e) {
	cpBodySetPosition(e->body, cpv(e->position.X, e->position.Y));
	cpBodySetAngle(e->body, e->angle);
	cpBodySetVelocity(e->body, cpv(e->velocity.X, e->velocity.Y));
	cpBodySetAngularVelocity(e->body, e->angular_velocity);
	cpBodySetForce(e->body, cpv(e->force.X, e->force.Y));
	cpBodySetTorque(e->body, e->torque);
}

static void physics_load(physics_entity_t *e) {
	cpVect position = cpBodyGetPosition(e->body);
	e->position = HMM_Vec2((float)position.x, (float)position.y);
	e->angle = (float)cpBodyGetAngle(e->body);
	cpVect velocity = cpBodyGetVelocity(e->body);
	e->velocity = HMM_Vec2((float)velocity.x, (float)velocity.y);
	e->angular_velocity = (float)cpBodyGetAngularVelocity(e->body);
	cpVect force = cpBodyGetForce(e->body);
	e->force = HMM_Vec2((float)force.x, (float)force.y);
	e->torque = (float)cpBodyGetTorque(e->body);
}

static void physics_velocity_func(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) {
	const physics_entity_t *physics_entity = (const physics_entity_t *)cpBodyGetUserData(body);

	cpBodyUpdateVelocity(body, gravity, damping, dt);

	if (physics_entity->max_speed) {
		cpFloat speed = cpvlength(body->v);
		cpFloat speed_percent = speed / physics_entity->max_speed;
		if (speed_percent > 1) {
			body->v = cpvmult(body->v, 1 / speed_percent);
		}
	}

	if (physics_entity->max_spin) {
		cpFloat spin = cpfabs(body->w);
		cpFloat spin_percent = spin / physics_entity->max_spin;
		if (spin_percent > 1) {
			body->w = body->w * (1 / spin_percent);
		}
	}
}

static cpBool physics_collision_begin_func(cpArbiter *arb, cpSpace *space, cpDataPointer userData) {
	cpShape *s1, *s2;
	cpArbiterGetShapes(arb, &s1, &s2);

	physics_entity_t *e1 = (physics_entity_t *)cpShapeGetUserData(s1);
	physics_entity_t *e2 = (physics_entity_t *)cpShapeGetUserData(s2);

	if (e1 && e2) {
		int absent1, absent2;
		kh_put(physics_entity_contact_map, &e1->contact_map, e2->id, &absent1);
		kh_put(physics_entity_contact_map, &e2->contact_map, e1->id, &absent2);
	}

	cpBool respond = e1->response_mask & e2->response_mask;
	return respond;
}
static void physics_collision_separate_func(cpArbiter *arb, cpSpace *space, cpDataPointer userData) {
	cpShape *s1, *s2;
	cpArbiterGetShapes(arb, &s1, &s2);

	physics_entity_t *e1 = (physics_entity_t *)cpShapeGetUserData(s1);
	physics_entity_t *e2 = (physics_entity_t *)cpShapeGetUserData(s2);

	if (e1 && e2) {
		khint_t k1 = kh_get(physics_entity_contact_map, &e1->contact_map, e2->id);
		khint_t k2 = kh_get(physics_entity_contact_map, &e2->contact_map, e1->id);
		kh_del(physics_entity_contact_map, &e1->contact_map, k1);
		kh_del(physics_entity_contact_map, &e2->contact_map, k2);
	}
}