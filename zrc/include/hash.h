#ifndef _HASH_H_
#define _HASH_H_

#include <khash.h>

#define kh_resize_fit(name, h, s) kh_resize(name, h, (int)ceil((s) / __ac_HASH_UPPER))

#define kh_foreach_value_ptr(h, vvar, code) { khint_t __i;		\
	for (__i = kh_begin(h); __i != kh_end(h); ++__i) {		\
		if (!kh_exist(h,__i)) continue;						\
		(vvar) = &kh_val(h,__i);								\
		code;												\
	} }

#endif