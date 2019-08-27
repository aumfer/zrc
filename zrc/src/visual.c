#include <visual.h>
#include <assert.h>

void visual_create(visual_t *visual) {

}
void visual_destroy(visual_t *visual) {

}

void visual_add(visual_t *visual, visual_entity_t *visual_entity) {
	int absent;
	khint_t k = kh_put(visual_map, &visual->map, visual_entity->id, &absent);
	kh_val(&visual->map, k) = *visual_entity;
}
void visual_remove(visual_t *visual, id_t id) {
	khint_t k = kh_get(visual_map, &visual->map, id);
	assert(k != kh_end(&visual->map));
	kh_del(visual_map, &visual->map, k);
}

void visual_update(visual_t *visual, physics_t *physics, float dt) {
	visual_entity_t *visual_entity;
	kh_foreach_value_ptr(&visual->map, visual_entity, {
		khint_t k = kh_get(physics_map, &physics->map, visual_entity->id);
		if (k == kh_end(&physics->map)) {
			continue;
		}
		physics_entity_t *physics_entity = &kh_val(&physics->map, k);
		
		float radius = physics_entity->radius;
		visual_entity->radius = radius;
		hmm_vec2 position = HMM_AddVec2(physics_entity->position, HMM_MultiplyVec2f(physics_entity->velocity, dt));
		visual_entity->position = position;
		hmm_mat4 translate = HMM_Translate(HMM_Vec3(position.X, position.Y, 0));
		float angle = physics_entity->angle + physics_entity->angular_velocity * dt;
		visual_entity->angle = angle;

		hmm_mat4 rotate = HMM_Rotate(angle, HMM_Vec3(0, 0, 1));
		hmm_vec3 size = HMM_Vec3(physics_entity->radius * 2, physics_entity->radius * 2, 1);
		hmm_mat4 scale = HMM_Scale(size);
		hmm_mat4 transform = HMM_MultiplyMat4(translate, rotate);
		transform = HMM_MultiplyMat4(transform, scale);
		visual_entity->transform = transform;

		hmm_vec2 mins = HMM_Vec2(position.X - radius, position.Y - radius);
		hmm_vec2 maxs = HMM_Vec2(position.X + radius, position.Y + radius);
		visual_entity->mins = mins;
		visual_entity->maxs = maxs;
	});
}