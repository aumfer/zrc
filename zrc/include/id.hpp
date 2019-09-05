#ifndef _ID_H_
#define _ID_H_

typedef struct id {
    int a, b, c, d;
} id_t;

const id ID_EMPTY = {};

constexpr int id_hash_func(id id) {
	return id.a ^ id.b ^ id.c ^ id.d;
}
constexpr int id_hash_equal(id l, id r) {
	return ((l).a == (r).a && (l).b == (r).b && (l).c == (r).c && (l).d == (r).d);
}

constexpr bool operator ==(const id &a, const id &b) {
	return id_hash_equal(a, b);
}
constexpr bool operator !=(const id &a, const id &b) {
	return !id_hash_equal(a, b);
}

struct id_hasher {
	constexpr int operator()(id id) const noexcept {
		return id_hash_func(id);
	}
};

struct id_eqer {
	constexpr bool operator()(id l, id r) const noexcept {
		return id_hash_equal(l, r);
	}
};

void id_create(id_t *);

#endif