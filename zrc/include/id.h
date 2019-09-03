#ifndef _ID_H_
#define _ID_H_

typedef struct id {
    int a, b, c, d;
} id_t;

constexpr int id_hash_func(id id) {
	return id.a ^ id.b ^ id.c ^ id.d;
}
constexpr int id_hash_equal(id l, id r) {
	return ((l).a == (r).a && (l).b == (r).b && (l).c == (r).c && (l).d == (r).d);
}

void id_create(id_t *);

#endif