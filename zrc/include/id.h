#ifndef _ID_H_
#define _ID_H_

typedef struct id {
    int a, b, c, d;
} id_t;

#define id_hash_func(key) (khint32_t)((key).a ^ (key).b ^ (key).c ^ (key).d)
#define id_hash_equal(l, r) ((l).a == (r).a && (l).b == (r).b && (l).c == (r).c && (l).d == (r).d)

void id_create(id_t *);

#endif