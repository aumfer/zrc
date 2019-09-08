#pragma once

#include <id.hpp>
#include <unordered_set>

struct zset {
	//zset(const zset&) = delete;
	zset& operator=(const zset&) = delete;

	void add(id id) {
		set.insert(id);
	}
	void del(id id) {
		set.erase(id);
	}
	bool has(id id) {
		return set.find(id) != set.end();
	}
	void clear() {
		set.clear();
	}

	template<typename R>
	void foreach(const R& receiver) const {
		for (id id : set) {
			receiver(id);
		}
	}

private:
	std::unordered_set<id, id_hasher, id_eqer> set;
};