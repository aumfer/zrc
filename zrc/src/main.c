#include <zrc.h>
#include <tinycthread.h>
#include <color.h>

static zrc_t zrc;

int main(int argc, char **argv) {
	zrc_create(&zrc);

	zrc.camera.position = HMM_Vec2(WORLD_HALF, WORLD_HALF);
	//zrc.camera.zoom = 16;
	zrc.camera.zoom = 512;
	//zrc.camera.zoom = WORLD_SIZE;

	for (int i = 0; i < 2048; ++i) {
		id_t id;
		id_create(&id);

		physics_add(&zrc.physics, &(physics_entity_t){
			.id = id,
			.type = CP_BODY_TYPE_DYNAMIC,
			.collide_flags = CP_ALL_CATEGORIES,
			.collide_mask = CP_ALL_CATEGORIES,
			.response_mask = CP_ALL_CATEGORIES,
			//.radius = fmodf(randf(), 9.9f) + 0.1f,
			.radius = 0.1f,
			.position = HMM_Vec2(randf() * WORLD_SIZE, randf() * WORLD_SIZE),
			.angle = randf() * (HMM_PI32 * 2),

			//.velocity = cpv(rand() % 10, rand() % 10)
			.force = HMM_Vec2((randf()-0.5f)*1000, (randf() - 0.5f) * 1000)
		});
		
		visual_entity_t visual_entity = {
			.id = id
		};
		color_random(visual_entity.color, 255);
		visual_add(&zrc.visual, &visual_entity);
	}

	do {
		zrc_update(&zrc);
		thrd_yield();
	} while (!zrc.ui.should_close);

	zrc_destroy(&zrc);

    return 0;
}
