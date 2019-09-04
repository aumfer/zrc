#include <physics.h>
#include <assert.h>

static void physics_store(const physics_entity_t &);
static void physics_load(physics_entity_t *);
static void physics_velocity_func(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
static cpBool physics_collision_begin_func(cpArbiter *arb, cpSpace *space, cpDataPointer userData);
static void physics_collision_separate_func(cpArbiter *arb, cpSpace *space, cpDataPointer userData);

physics::physics() {
	space = cpSpaceNew();
	cpSpaceSetUserData(space, this);

	collision_handler = cpSpaceAddDefaultCollisionHandler(space);

	collision_handler->beginFunc = physics_collision_begin_func;
	collision_handler->separateFunc = physics_collision_separate_func;
}
physics::~physics() {

}

void physics::add(const physics_entity &create) {
	physics_entity physics_entity = create;

	//cpFloat mass = physics_entity.radius * physics_entity.radius;
	cpFloat mass = 1;
	cpFloat moment = cpMomentForCircle(mass, 0, physics_entity.radius*2, cpvzero);
	physics_entity.body = cpBodyAlloc();
	cpBodyInit(physics_entity.body, mass, moment);
	cpBodySetType(physics_entity.body, physics_entity.type);
	physics_store(physics_entity);
	cpBodySetVelocityUpdateFunc(physics_entity.body, physics_velocity_func);

	physics_entity.circle = cpCircleShapeAlloc();
	cpVect offset = cpvzero;
	//cpVect offset = cpv(physics_entity.radius, physics_entity.radius);
	cpCircleShapeInit(physics_entity.circle, physics_entity.body, physics_entity.radius, offset);
	cpShapeFilter filter;
	filter.group = id_hash_func(physics_entity.id);
	filter.categories = physics_entity.collide_flags;
	filter.mask = physics_entity.collide_mask;
	cpShapeSetFilter(&physics_entity.circle->shape, filter);
	//cpShapeSetElasticity(&e.circle.shape, 0.0f);
	//cpShapeSetFriction(&e.circle.shape, 0.7f);

	physics_entity_t &created = zsys::add(physics_entity);
	cpBodySetUserData(created.body, &created);
	cpShapeSetUserData(&created.circle->shape, &created);
	cpSpaceAddBody(space, created.body);
	cpSpaceAddShape(space, &created.circle->shape);
}
void physics::del(id id) {
	physics_entity *physics_entity = get(id);
	
	cpSpaceRemoveShape(space, &physics_entity->circle->shape);
	cpSpaceRemoveBody(space, physics_entity->body);
	cpShapeFree(&physics_entity->circle->shape);
	cpBodyFree(physics_entity->body);

	zsys::del(id);
}

void physics::update(float dt) {
	foreach([&](const physics_entity &physics_entity) {
		physics_store(physics_entity);
	});
	cpSpaceStep(space, dt);
	foreach([&](physics_entity &physics_entity) {
		physics_load(&physics_entity);
	});
}

static void physics_store(const physics_entity_t &e) {
	cpBodySetPosition(e.body, cpv(e.position.x, e.position.y));
	cpBodySetAngle(e.body, e.angle);
	cpBodySetVelocity(e.body, cpv(e.velocity.x, e.velocity.y));
	cpBodySetAngularVelocity(e.body, e.angular_velocity);
	cpBodySetForce(e.body, cpv(e.force.x, e.force.y));
	cpBodySetTorque(e.body, e.torque);
}

static void physics_load(physics_entity_t *e) {
	cpVect position = cpBodyGetPosition(e->body);
	e->position = glm::vec2((float)position.x, (float)position.y);
	e->angle = (float)cpBodyGetAngle(e->body);
	cpVect velocity = cpBodyGetVelocity(e->body);
	e->velocity = glm::vec2((float)velocity.x, (float)velocity.y);
	e->angular_velocity = (float)cpBodyGetAngularVelocity(e->body);
	cpVect force = cpBodyGetForce(e->body);
	e->force = glm::vec2((float)force.x, (float)force.y);
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