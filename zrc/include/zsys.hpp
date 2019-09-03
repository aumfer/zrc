#pragma once

#include <hash.h>
#include <id.h>
#include <assert.h>

template<typename E, int MAX_ENTITIES=0>
struct zsys {
	zsys() {
		if (MAX_ENTITIES) {
			int size = (int)ceil(MAX_ENTITIES / __ac_HASH_UPPER);
			map.resize(size);
		}
	}

	E &add(const E &e) {
		assert(!MAX_ENTITIES || map.n_occupied < MAX_ENTITIES);

		int absent;
		khint_t k = map.put(e.id, &absent);
		return kh_val(&map, k) = e;
	}
	void del(id id) {
		khint_t k = map.get(id);
		map.del(k);
	}
	E *get(id id) {
		khint_t k = map.get(id);
		return &kh_val(&map, k);
	}
	const E *get(id id) const {
		khint_t k = map.get(id);
		return &kh_val(&map, k);
	}

	template<typename R>
	void foreach(const R& receiver) {
		E *e;
		kh_foreach_value_ptr(&map, e, {
			receiver(*e);
		});
	}
	template<typename R>
	void foreach(const R& receiver) const  {
		const E *e;
		kh_foreach_value_ptr(&map, e, {
			receiver(*e);
		});
	}

private:
	kh_template<id, E, id_hash_func, id_hash_equal> map;
};