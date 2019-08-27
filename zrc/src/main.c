#include <zrc.h>
#include <tinycthread.h>
#include <color.h>

static zrc_t zrc;

int main(int argc, char **argv) {
	zrc_create(&zrc);

	zrc.camera.position = HMM_Vec2(MAP_HALF, MAP_HALF);
	zrc.camera.zoom = 0.01f;
	//zrc.camera.zoom = 1;

	for (int i = 0; i < 1024; ++i) {
		id_t id;
		id_create(&id);

		physics_add(&zrc.physics, &(physics_entity_t){
			.id = id,
			.type = CP_BODY_TYPE_DYNAMIC,
			.collide_flags = CP_ALL_CATEGORIES,
			.collide_mask = CP_ALL_CATEGORIES,
			.response_mask = CP_ALL_CATEGORIES,
			.radius = fmodf(randf(), 9) + 1,
			.position = HMM_Vec2(fmodf(randf(), MAP_SIZE), fmodf(randf(), MAP_SIZE)),
			.angle = fmodf(randf(), (HMM_PI32 * 2)),

			//.velocity = cpv(rand() % 10, rand() % 10)
			.force = HMM_Vec2(fmodf(randf(), 1000), fmodf(randf(), 1000))
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
