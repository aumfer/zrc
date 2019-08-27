#ifndef _LSM_H_
#define _LSM_H_

#include <HandmadeMath.h>
#include <stdlib.h>

#define randf() ((float)rand())

typedef struct hmm_ivec2 {
	int X, Y;
} hmm_ivec2;

inline int lsm_index2d(int x, int y, int w, int h) {
	int index = y +
		x * w;
	return index;
}
inline int lsm_index3d(int x, int y, int z, int w, int h, int d) {
	int index = z +
		y * h +
		x * w * h;
	return index;
}

inline hmm_ivec2 HMM_IVec2(int x, int y) {
	return (hmm_ivec2) {
		.X = x, .Y = x
	};
}
inline int HMM_EqualsIVec2(hmm_ivec2 left, hmm_ivec2 right) {
	return left.X == right.X && left.Y == right.Y;
}

#endif