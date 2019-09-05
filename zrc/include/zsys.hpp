#pragma once

#include <id.hpp>
#include <assert.h>
#include <unordered_map>

template<typename E, int MAX_ENTITIES=0>
struct zsys {
	zsys() {
		if (MAX_ENTITIES) {
			//int size = (int)ceil(MAX_ENTITIES / map.max_load_factor());
			//map.rehash(size);
			map.reserve(MAX_ENTITIES);
		}
	}

	E &add(const E &e) {
		assert(!MAX_ENTITIES || map.size() < MAX_ENTITIES);

		auto i = map.insert({ e.id, e });
		return i.first->second;
	}
	void del(id id) {
		map.erase(id);
	}
	E *get(id id) {
		return &map.find(id)->second;
	}
	const E *get(id id) const {
		return &map.find(id)->second;
	}

	template<typename R>
	void foreach(const R& receiver) {
		for (auto& n : map) {
			receiver(n.second);
		}
	}
	template<typename R>
	void foreach(const R& receiver) const  {
		for (const auto& n : map) {
			receiver(n.second);
		}
	}

private:
	std::unordered_map<id, E, id_hasher, id_eqer> map;
};