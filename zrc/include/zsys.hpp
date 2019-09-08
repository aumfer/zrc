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
	//zsys(const zsys&) = delete;
	zsys& operator=(const zsys&) = delete;

	E &add(const E &e) {
		assert(!MAX_ENTITIES || map.size() < MAX_ENTITIES);

		auto p = std::make_pair(e.id, std::move(e));
		auto i = map.emplace(p);
		return i.first->second;
	}
	void del(id id) {
		map.erase(id);
	}
	bool has(id id) {
		return map.find(id) != map.end();
	}
	void clear() {
		map.clear();
	}
	E *get(id id) {
		return &map.find(id)->second;
	}
	const E *get(id id) const {
		return &map.find(id)->second;
	}
	
	template<typename R>
	void get(id id, const R &receiver) {
		E *e = get(id);
		if (e) {
			receiver(*e);
		}
	}
	template<typename R>
	void get(id id, const R &receiver) const {
		const E *e = get(id);
		if (e) {
			receiver(*e);
		}
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
