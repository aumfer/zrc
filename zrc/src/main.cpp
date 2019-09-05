#include <zrc.hpp>
#include <tinycthread.h>
#include <color.h>

static zrc_t game;

int main(int argc, char **argv) {
	for (int i = 0; i < 1024; ++i) {
		id_t id;
		id_create(&id);

		physics_entity_t physics_entity = {};
		physics_entity.id = id;
		physics_entity.type = CP_BODY_TYPE_DYNAMIC;
		physics_entity.collide_flags = CP_ALL_CATEGORIES;
		physics_entity.collide_mask = CP_ALL_CATEGORIES;
		physics_entity.response_mask = CP_ALL_CATEGORIES;
		physics_entity.radius = glm::linearRand<float>(1, 10);
		//physics_entity//.radius = 0.5f;
		physics_entity.position = glm::linearRand(glm::vec2(0), glm::vec2(WORLD_SIZE));
		physics_entity.angle = glm::linearRand<float>(0, glm::pi<float>() * 2);

		//physics_entity//.velocity = cpv(rand() % 10, rand() % 10)
		physics_entity.force = glm::linearRand(glm::vec2(-1000), glm::vec2(1000));
		physics_entity.torque = glm::linearRand<float>(-1000, 1000);
		game.physics.add(physics_entity);
		
		visual_entity_t visual_entity = {};
		visual_entity.id = id;
		color_random(glm::value_ptr(visual_entity.color), 255);
		game.visual.add(visual_entity);
	}

	thrd_t update;
	thrd_create(&update, [](void *arg) {
		zrc &game = *(zrc *)arg;
		do {
			game.update();
			thrd_yield();
		} while (!game.ui.should_close);
		return 0;
	}, &game);

	do {
		game.draw();
		thrd_yield();
	} while (!game.ui.should_close);

	int res;
	thrd_join(update, &res);
    return 0;
}
