#ifndef _LSM_H_
#define _LSM_H_

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <stdlib.h>

#define randf() ((float)rand() / RAND_MAX)


constexpr int lsm_index2d(int x, int y, int w, int h) {
	int index = y +
		x * w;
	return index;
}
constexpr int lsm_index3d(int x, int y, int z, int w, int h, int d) {
	int index = z +
		y * h +
		x * w * h;
	return index;
}

template<typename T>
T lsm_normalize_safe(const T &x) {
	float l = glm::length2(x);
	return l ? glm::normalize(x) : x;
}

#endif