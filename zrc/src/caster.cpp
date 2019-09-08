#include <caster.hpp>

void caster::update(const ability &ability, float dt) {
	foreach([&](caster_entity &caster_entity) {
		caster_entity.abilities.foreach([&](caster_ability &caster_ability) {
			caster_ability.downtime += dt;

			if (caster_ability.cast) {
				ability.get(caster_ability.id, [&](const ability_entity &ability_entity) {
					if (caster_ability.downtime >= ability_entity.cooldown) {
						ability_entity.cast(caster_entity.id, caster_ability.target, 0);
						caster_ability.downtime = 0;
					}
				});
			}
		});
	});
}