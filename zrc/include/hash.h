#ifndef _HASH_H_
#define _HASH_H_

extern "C" {
#include <khash.h>
}

#define kh_resize_fit(name, h, s) kh_resize(name, h, (int)ceil((s) / __ac_HASH_UPPER))

#define kh_foreach_ptr(h, kvar, vvar, code) { khint_t __i;		\
	for (__i = kh_begin(h); __i != kh_end(h); ++__i) {		\
		if (!kh_exist(h,__i)) continue;						\
		(kvar) = kh_key(h,__i);								\
		(vvar) = &kh_val(h,__i);								\
		code;												\
	} }

#define kh_foreach_value_ptr(h, vvar, code) { khint_t __i;		\
	for (__i = kh_begin(h); __i != kh_end(h); ++__i) {		\
		if (!kh_exist(h,__i)) continue;						\
		(vvar) = &kh_val(h,__i);								\
		code;												\
	} }

template<typename khkey_t, typename khval_t, int(*__hash_func)(khkey_t), int(*__hash_equal)(khkey_t,khkey_t), int kh_is_map=1>
struct kh_template {
	khint_t n_buckets, size, n_occupied, upper_bound;
	khint32_t *flags;
	khkey_t *keys;
	khval_t *vals;

	kh_template() {

	}
	~kh_template() {
		kfree((void *)keys); kfree(flags);
		kfree((void *)vals);
	}
	void clear() {
		if (flags) {
				memset(flags, 0xaa, __ac_fsize(n_buckets) * sizeof(khint32_t));
				size = n_occupied = 0;
		}
	}

	khint_t get(khkey_t key) {
		if (n_buckets) {
			khint_t k, i, last, mask, step = 0;
			mask = n_buckets - 1;
			k = __hash_func(key); i = k & mask;
			last = i;
			while (!__ac_isempty(flags, i) && (__ac_isdel(flags, i) || !__hash_equal(keys[i], key))) {
				i = (i + (++step)) & mask;
				if (i == last) return n_buckets;
			}
			return __ac_iseither(flags, i)? n_buckets : i;
		} else return 0;
	}
	int resize(khint_t new_n_buckets)
	{ /* This function uses 0.25*n_buckets bytes of working space instead of [sizeof(key_t+val_t)+.25]*n_buckets. */
		khint32_t *new_flags = 0;
		khint_t j = 1;
		{
			kroundup32(new_n_buckets);
			if (new_n_buckets < 4) new_n_buckets = 4;
			if (size >= (khint_t)(new_n_buckets * __ac_HASH_UPPER + 0.5)) j = 0;	/* requested size is too small */
			else { /* hash table size to be changed (shrink or expand); rehash */
				new_flags = (khint32_t*)kmalloc(__ac_fsize(new_n_buckets) * sizeof(khint32_t));
				if (!new_flags) return -1;
				memset(new_flags, 0xaa, __ac_fsize(new_n_buckets) * sizeof(khint32_t));
				if (n_buckets < new_n_buckets) {	/* expand */		\
					khkey_t *new_keys = (khkey_t*)krealloc((void *)keys, new_n_buckets * sizeof(khkey_t));
					if (!new_keys) { kfree(new_flags); return -1; }
					keys = new_keys;
					if (kh_is_map) {
						khval_t *new_vals = (khval_t*)krealloc((void *)vals, new_n_buckets * sizeof(khval_t));
						if (!new_vals) { kfree(new_flags); return -1; }
						vals = new_vals;
					}
				} /* otherwise shrink */
			}
		}
		if (j) { /* rehashing is needed */
			for (j = 0; j != n_buckets; ++j) {
				if (__ac_iseither(flags, j) == 0) {
					khkey_t key = keys[j];
					khval_t val;
					khint_t new_mask;
					new_mask = new_n_buckets - 1;
					if (kh_is_map) val = vals[j];
					__ac_set_isdel_true(flags, j);
					while (1) { /* kick-out process; sort of like in Cuckoo hashing */
						khint_t k, i, step = 0;
						k = __hash_func(key);
						i = k & new_mask;
						while (!__ac_isempty(new_flags, i)) i = (i + (++step)) & new_mask;
						__ac_set_isempty_false(new_flags, i);
						if (i < n_buckets && __ac_iseither(flags, i) == 0) { /* kick out the existing element */
							{ khkey_t tmp = keys[i]; keys[i] = key; key = tmp; }
							if (kh_is_map) { khval_t tmp = vals[i]; vals[i] = val; val = tmp; }
							__ac_set_isdel_true(flags, i); /* mark it as deleted in the old hash table */
						} else { /* write the element and jump out of the loop */
							keys[i] = key;
							if (kh_is_map) vals[i] = val;
							break;
						}
					}
				}
			}
			if (n_buckets > new_n_buckets) { /* shrink the hash table */
				keys = (khkey_t*)krealloc((void *)keys, new_n_buckets * sizeof(khkey_t));
				if (kh_is_map) vals = (khval_t*)krealloc((void *)vals, new_n_buckets * sizeof(khval_t));
			}
			kfree(flags); /* free the working space */
			flags = new_flags;
			n_buckets = new_n_buckets;
			n_occupied = size;
			upper_bound = (khint_t)(n_buckets * __ac_HASH_UPPER + 0.5);
		}
		return 0;
	}
	khint_t put(khkey_t key, int *ret) {
		khint_t x;
		if (n_occupied >= upper_bound) { /* update the hash table */
			if (n_buckets > (size<<1)) {
				if (resize(n_buckets - 1) < 0) { /* clear "deleted" elements */
					*ret = -1; return n_buckets;
				}
			} else if (resize(n_buckets + 1) < 0) { /* expand the hash table */
				*ret = -1; return n_buckets;
			}
		} /* TODO: to implement automatically shrinking; resize() already support shrinking */
		{
			khint_t k, i, site, last, mask = n_buckets - 1, step = 0;
			x = site = n_buckets; k = __hash_func(key); i = k & mask;
			if (__ac_isempty(flags, i)) x = i; /* for speed up */
			else {
				last = i;
				while (!__ac_isempty(flags, i) && (__ac_isdel(flags, i) || !__hash_equal(keys[i], key))) {
					if (__ac_isdel(flags, i)) site = i;
					i = (i + (++step)) & mask;
					if (i == last) { x = site; break; }
				}
				if (x == n_buckets) {
					if (__ac_isempty(flags, i) && site != n_buckets) x = site;
					else x = i;
				}
			}
		}
		if (__ac_isempty(flags, x)) { /* not present at all */
			keys[x] = key;
			__ac_set_isboth_false(flags, x);
			++size; ++n_occupied;
			*ret = 1;
		} else if (__ac_isdel(flags, x)) { /* deleted */
			keys[x] = key;
			__ac_set_isboth_false(flags, x);
			++size;
			*ret = 2;
		} else *ret = 0; /* Don't touch keys[x] if present and not deleted */
		return x;
	}
	void del(khint_t x) {
		if (x != n_buckets && !__ac_iseither(flags, x)) {
			__ac_set_isdel_true(flags, x);
			--size;	
		}
	}
	int exist(khint_t x) {
		return !__ac_iseither(flags, (x));
	}

	template<typename R>
	void foreach(const R& receiver) {
		khint_t __i;
		for (__i = 0; __i != num_buckets; ++__i) {
			if (!exist(__i)) continue;
			khkey_t key = keys[__i];
			khval_t &val = vals[__i];
			receiver(key, val);
		}
	}
};

#endif