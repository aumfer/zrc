GLSL(
const float M_PI = 3.14159265358979323846;
const float M_PHI = 1.6180339887498948482045868343656;
const float FLT_EPSILON = 9.99999974737875E-06;
const float FLT_MAX = 3.402823466e+38F;

float unorm(float v) {
	return v * 0.5 + 0.5;
}
vec2 unorm(vec2 v) {
	return v * 0.5 + 0.5;
}
vec3 unorm(vec3 v) {
	return v * 0.5 + 0.5;
}
vec4 unorm(vec4 v) {
	return v * 0.5 + 0.5;
}
float snorm(float v) {
    return v * 2.0 - 1.0;
}
vec2 snorm(vec2 v) {
	return v * 2.0 - 1.0;
}
vec3 snorm(vec3 v) {
	return v * 2.0 - 1.0;
}
vec4 snorm(vec4 v) {
	return v * 2.0 - 1.0;
}

float square(float X) {
	return X * X;
}

float mincomp(vec2 v) { return min(v.x, v.y); }
float maxcomp(vec2 v) { return max(v.x, v.y); }
float mincomp(vec3 v) { return min(min(v.x, v.y), v.z); }
float maxcomp(vec3 v) { return max(max(v.x, v.y), v.z); }

float smin(float a, float b, float k) {
    // http://iquilezles.org/www/articles/smin/smin.htm
    float res = exp(-k * a) + exp(-k * b);
    return -log(res) / k;
}

float cubemap_layerface(float layer_orig) {
	float layer = floor(layer_orig / 6.0);
	float face = layer_orig - (layer * 6.0);
	return face;
}

void swap(inout float a, inout float b) {
	float t = a;
	a = b;
	b = t;
}
float gamma(float n) {
	return (n * FLT_EPSILON) / (1.0 - n * FLT_EPSILON);
}

mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
	float tanHalfFovy = tan(fovy / 2);

	mat4 Result = mat4(0);
	Result[0][0] = 1 / (aspect * tanHalfFovy);
	Result[1][1] = 1 / (tanHalfFovy);
	Result[2][3] = - 1;
	Result[2][2] = - (zFar + zNear) / (zFar - zNear);
	Result[3][2] = - (2 * zFar * zNear) / (zFar - zNear);

	return Result;
}

mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
	mat4 Result = mat4(1);
	Result[0][0] = 2 / (right - left);
	Result[1][1] = 2 / (top - bottom);
	Result[3][0] = - (right + left) / (right - left);
	Result[3][1] = - (top + bottom) / (top - bottom);
	Result[2][2] = - 2 / (zFar - zNear);
	Result[3][2] = - (zFar + zNear) / (zFar - zNear);
	return Result;
}

mat4 lookAt(vec3 eye, vec3 center, vec3 up) {
	vec3 f = normalize(center - eye);
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);

	mat4 Result = mat4(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] =-f.x;
	Result[1][2] =-f.y;
	Result[2][2] =-f.z;
	Result[3][0] =-dot(s, eye);
	Result[3][1] =-dot(u, eye);
	Result[3][2] = dot(f, eye);
	return Result;
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

vec3 rotateX(vec3 v, float angle) {
	vec3 Result = v;
	float Cos = cos(angle);
	float Sin = sin(angle);

	Result.y = v.y * Cos - v.z * Sin;
	Result.z = v.y * Sin + v.z * Cos;
	return Result;
}
vec4 rotateX(vec4 v, float angle) {
	vec4 Result = v;
	float Cos = cos(angle);
	float Sin = sin(angle);

	Result.y = v.y * Cos - v.z * Sin;
	Result.z = v.y * Sin + v.z * Cos;
	return Result;
}

vec3 rotateY(vec3 v, float angle) {
	vec3 Result = v;
	float Cos = cos(angle);
	float Sin = sin(angle);

	Result.x =  v.x * Cos + v.z * Sin;
	Result.z = -v.x * Sin + v.z * Cos;
	return Result;
}
vec4 rotateY(vec4 v, float angle) {
	vec4 Result = v;
	float Cos = cos(angle);
	float Sin = sin(angle);

	Result.x =  v.x * Cos + v.z * Sin;
	Result.z = -v.x * Sin + v.z * Cos;
	return Result;
}

vec2 rotateZ(vec2 v, float angle) {
	float Cos = cos(angle);
	float Sin = sin(angle);

	vec2 Result;
	Result.x = v.x * Cos - v.y * Sin;
	Result.y = v.x * Sin + v.y * Cos;
	return Result;
}
vec3 rotateZ(vec3 v, float angle) {
	vec3 Result = v;
	float Cos = cos(angle);
	float Sin = sin(angle);

	Result.x = v.x * Cos - v.y * Sin;
	Result.y = v.x * Sin + v.y * Cos;
	return Result;
}
vec4 rotateZ(vec4 v, float angle) {
	vec4 Result = v;
	float Cos = cos(angle);
	float Sin = sin(angle);

	Result.x = v.x * Cos - v.y * Sin;
	Result.y = v.x * Sin + v.y * Cos;
	return Result;
}

//==========================================================================================
// indexing
//==========================================================================================

int index2d(ivec2 p, ivec2 bounds) {
	int index = p.x +
				p.y * bounds.x;
	return index;
}
int index3d(ivec3 p, ivec3 bounds) {
	int index = p.x +
				p.y * bounds.x +
				p.z * bounds.x * bounds.y;
	return index;
}
int index4d(ivec4 p, ivec4 bounds) {
	int index = p.x +
				p.y * bounds.x +
				p.z * bounds.x * bounds.y +
				p.w * bounds.x * bounds.y * bounds.z;
	return index;
}

//==========================================================================================
// hashes
//==========================================================================================

float hash( float n ){return fract(sin(n)*43758.5453);}

float hash1(vec2 p) {
	p = 50.0*fract(p*0.3183099);
	return fract(p.x*p.y*(p.x + p.y));
}

float hash1(float n) {
	return fract(n*17.0*fract(n*0.3183099));
}

vec2 hash2(float n) { return fract(sin(vec2(n, n + 1.0))*vec2(43758.5453123, 22578.1459123)); }

vec2 hash2(vec2 p) {
	const vec2 k = vec2(0.3183099, 0.3678794);
	p = p*k + k.yx;
	return fract(16.0 * k*fract(p.x*p.y*(p.x + p.y)));
}

//==========================================================================================
// rng
//==========================================================================================

// http://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) {
	x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}
// Compound versions of the hashing algorithm I whipped together.
uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct(uint m) {
	const uint ieeeMantissa = 0x007FFFFFu;	// binary32 mantissa bitmask
	const uint ieeeOne = 0x3F800000u;		// 1.0 in IEEE binary32

	m &= ieeeMantissa;						// Keep only mantissa bits (fractional part)
	m |= ieeeOne;							// Add fractional part to 1.0

	float  f = uintBitsToFloat(m);			// Range [1:2]
	return f - 1.0;							// Range [0:1]
}

uniform uvec4 random_seed;
uvec4 seed = random_seed;
void random_init1ui(uint x) {
	seed ^= uvec4(x, ~x, -x, -~x);
}
void random_init2ui(uvec2 x) {
	seed ^= uvec4(x.x, x.y, ~x.x, ~x.y);
}
void random_init2f(vec2 v) {
	uvec2 x = floatBitsToUint(v);
	seed ^= uvec4(x.x, x.y, ~x.x, ~x.y);
}
void random_init3f(vec3 v) {
	uvec3 x = floatBitsToUint(v);
	seed ^= uvec4(x.x, x.y, x.z, x.x ^ x.y ^ x.z);
}
void random_init4f(vec4 v) {
	uvec4 x = floatBitsToUint(v);
	seed ^= uvec4(x.x, x.y, x.z, x.w);
}
float random1f() {
	return floatConstruct(hash(seed++));
}
vec2 random2f() {
	return vec2(random1f(), random1f());
}
vec3 random3f() {
	return vec3(random1f(), random1f(), random1f());
}
float normal1f() {
	return sqrt(random1f()) * cos(random1f() * M_PI * 2.0);
}
vec2 normal2f() {
	return vec2(normal1f(), normal1f());
}

//==========================================================================================
// noise
//==========================================================================================

//iq's ubiquitous 3d noise
float noise(vec3 p) {
	vec3 ip = floor(p);
    vec3 f = fract(p);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (ip.xy+vec2(37.0,17.0)*ip.z) + f.xy;
	vec2 rg = hash2(uv);
	return mix(rg.x, rg.y, f.z);
}

//==========================================================================================
// encoding
//==========================================================================================

// https://www.shadertoy.com/view/4t2XWK
float madfrac(float a, float b) { return a*b - floor(a*b); }
vec2  madfrac(vec2 a, float b) { return a*b - floor(a*b); }

float encode_normal(vec3 p, float n) {
	float phi = min(atan(p.y, p.x), M_PI), cosTheta = p.z;

	float k = max(2.0, floor(log(n * M_PI * sqrt(5.0) * (1.0 - cosTheta*cosTheta)) / log(M_PHI*M_PHI)));
	float Fk = pow(M_PHI, k) / sqrt(5.0);

	vec2 F = vec2(round(Fk), round(Fk * M_PHI));

	vec2 ka = -2.0*F / n;
	vec2 kb = 2.0*M_PI*madfrac(F + 1.0, M_PHI - 1.0) - 2.0*M_PI*(M_PHI - 1.0);
	mat2 iB = mat2(ka.y, -ka.x, -kb.y, kb.x) / (ka.y*kb.x - ka.x*kb.y);

	vec2 c = floor(iB * vec2(phi, cosTheta - (1.0 - 1.0 / n)));
	float d = 8.0;
	float j = 0.0;
	for (int s = 0; s<4; s++)
	{
		vec2 uv = vec2(float(s - 2 * (s / 2)), float(s / 2));

		float cosTheta = dot(ka, uv + c) + (1.0 - 1.0 / n);

		cosTheta = clamp(cosTheta, -1.0, 1.0)*2.0 - cosTheta;
		float i = floor(n*0.5 - cosTheta*n*0.5);
		float phi = 2.0*M_PI*madfrac(i, M_PHI - 1.0);
		cosTheta = 1.0 - (2.0*i + 1.0) / n;
		float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

		vec3 q = vec3(cos(phi)*sinTheta, sin(phi)*sinTheta, cosTheta);
		float squaredDistance = dot(q - p, q - p);
		if (squaredDistance < d)
		{
			d = squaredDistance;
			j = i;
		}
	}
	return j;
}

vec3 decode_normal(float i, float n) {
	float phi = 2.0*M_PI*madfrac(i, M_PHI);
	float zi = 1.0 - (2.0*i + 1.0) / n;
	float sinTheta = sqrt(1.0 - zi*zi);
	return vec3(cos(phi)*sinTheta, sin(phi)*sinTheta, zi);
}

//==========================================================================================
// sampling
//==========================================================================================

float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley2d(uint i, uint N) {
	return vec2(float(i) / float(N), radicalInverse_VdC(i));
}
vec2 hammersley2d(int i, int N) {
	return vec2(float(i) / float(N), radicalInverse_VdC(uint(i)));
}

// https://www.shadertoy.com/view/MsdGzl
vec3 cosineDirection(vec2 rnd, vec3 nor) {
	// compute basis from normal
	// see http://orbit.dtu.dk/fedora/objects/orbit:113874/datastreams/file_75b66578-222e-4c7d-abdf-f7e255100209/content
	// (link provided by nimitz)
	vec3 tc = vec3(1.0 + nor.z - nor.xy*nor.xy, -nor.x*nor.y) / (1.0 + nor.z);
	vec3 uu = vec3(tc.x, tc.z, -nor.x);
	vec3 vv = vec3(tc.z, tc.y, -nor.y);

	float u = rnd.x;
	float v = rnd.y;
	float a = M_PI * 2.0 * v;

	vec3 d = sqrt(u)*(cos(a)*uu + sin(a)*vv) + sqrt(1.0 - u)*nor;
	d = normalize(d);
	return d;
}

vec2 sampleSphericalMap(vec3 v) {
	const vec2 invAtan = vec2(0.1591, 0.3183);
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

//==========================================================================================
// distance
//==========================================================================================

vec2 sdTranslate(vec2 p, vec2 t) {
	return p - t;
}

float sdSmoothMerge(float d1, float d2, float k) {
    float h = clamp(0.5 + 0.5*(d2 - d1)/k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0-h);
}

float sdMerge(float d1, float d2) {
	return min(d1, d2);
}

float sdMergeExclude(float d1, float d2) {
	return min(max(-d1, d2), max(-d2, d1));
}

float sdSubstract(float d1, float d2) {
	return max(-d1, d2);
}

float sdIntersect(float d1, float d2) {
	return max(d1, d2);
}

float sdPie(vec2 p, float angle) {
	angle = radians(angle) / 2.0;
	vec2 n = vec2(cos(angle), sin(angle));
	return abs(p).x * n.x + p.y*n.y;
}

float sdCircle(vec2 p, float radius) {
	return length(p) - radius;
}

float sdTriangle(vec2 p, float radius) {
	return max(	abs(p).x * 0.866025 + 
			   	p.y * 0.5, -p.y) 
				-radius * 0.5;
}

float sdTriangle(vec2 p, float width, float height) {
	vec2 n = normalize(vec2(height, width / 2.0));
	return max(	abs(p).x*n.x + p.y*n.y - (height*n.y), -p.y);
}

float sdSemiCircle(vec2 p, float radius, float angle, float width) {
	width /= 2.0;
	radius -= width;
	return sdSubstract(sdPie(p, angle), 
					 abs(sdCircle(p, radius)) - width);
}

float sdBox(vec2 p, vec2 size, float radius) {
	size -= vec2(radius);
	vec2 d = abs(p) - size;
  	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

float sdLine(vec2 p, vec2 start, vec2 end, float width) {
	vec2 dir = start - end;
	float lngth = length(dir);
	dir /= lngth;
	vec2 proj = max(0.0, min(lngth, dot((start - p), dir))) * dir;
	return length( (start - p) - proj ) - (width / 2.0);
}

float length2(in vec2 v) { return dot(v, v); }

float sdSegmentSq(in vec2 p, in vec2 a, in vec2 b) {
	vec2 pa = p - a, ba = b - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
	return length2(pa - ba * h);
}

vec2 udBezierSq(vec2 p0, vec2 p1, vec2 p2, in vec2 p3, vec2 pos) {
	const int kNum = 50;
	vec2 res = vec2(1e10, 0.0);
	vec2 a = p0;
	for (int i = 1; i < kNum; i++)
	{
		float t = float(i) / float(kNum - 1);
		float s = 1.0 - t;
		vec2 b = p0 * s*s*s + p1 * 3.0*s*s*t + p2 * 3.0*s*t*t + p3 * t*t*t;
		float d = sdSegmentSq(pos, a, b);
		if (d < res.x) res = vec2(d, t);
		a = b;
	}

	return vec2(res.x, res.y);
}

vec2 hex_corner(vec2 center, float size, int i);
float sdHex(vec2 p, vec2 center, float size, float width) {
	float d = FLT_MAX;
	vec2 start = hex_corner(center, size, 0);
	for (int i = 1; i <= 6; ++i) {
		vec2 end = hex_corner(center, size, i % 6);
		d = min(d, sdLine(p, start, end, width));
		start = end;
	}
	return d;
}

//==========================================================================================
// intersection
//==========================================================================================

float isect_sphere(vec3 ro, vec3 rd, vec4 sph) {
	vec3 oc = ro - sph.xyz;
	float b = dot(oc, rd);
	float c = dot(oc, oc) - sph.w*sph.w;
	float h = b*b - c;
	if (h<0.0) {
		return -1.0;
	}
	h = sqrt(h);
	return -b - h;
}

float isect_plane(vec3 ro, vec3 rd, vec4 p) {
	float t = -(dot(ro, p.xyz) + p.w) / dot(rd, p.xyz);
	return t;
}

float minT(float a, float b) {
	const float zfar = -1.0;
	return (a<b)? zfar: (b<0.)? (a>0.)? a: zfar: b;
}

// https://www.shadertoy.com/view/XtGXRz
float isect_aabb(vec3 ro, vec3 rd, vec3 mins, vec3 maxs) {
	vec3 t1 = (mins - ro)/rd; //https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
	vec3 t2 = (maxs - ro)/rd;
	vec3 tn = min(t1, t2);
	vec3 tx = max(t1, t2);
    float d = minT(mincomp(tx),maxcomp(tn)); //minT calculates the minimum positive, if n/a then returns zfar
	return d;
}

float isect_triangle( vec3 orig
                       , vec3 dir
                       , vec3 vert0
                       , vec3 vert1
                       , vec3 vert2
                       , out float u
                       , out float v
                       ) {
    // Fast, Minimum Storage Ray-Triangle Intersection
    //
    // Tomas M�ller and Ben Trumbore. Fast, minimum storage ray-triangle intersection.
    // Journal of graphics tools, 2(1):21-28, 1997
    //
    // http://www.jcenligne.fr/download/little3d/
    //     jgt%20Fast,%20Minumum%20Storage%20Ray-Triangle%20Intersection.htm

    const float JGT_RAYTRI_EPSILON = 0.000001;
	float t;

    vec3 edge1, edge2, tvec, pvec, qvec;
    float det, inv_det;

    // Find vectors for two edges sharing vert0
    edge1 = vert1 - vert0;
    edge2 = vert2 - vert0;

    // Begin calculating determinant - also used to calculate U parameter
    pvec = cross(dir, edge2);

    // If determinant is near zero, ray lies in plane of triangle
    det = dot(edge1, pvec);

    if (det > -JGT_RAYTRI_EPSILON && det < JGT_RAYTRI_EPSILON) {
        return -1.0;
	}
    inv_det = 1.0 / det;

    // Calculate distance from vert0 to ray origin
    tvec = orig - vert0;

    // Calculate U parameter and test bounds
    u = dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0) {
        return -1.0;
	}

    // Prepare to test V parameter
    qvec = cross(tvec, edge1);

    // Calculate V parameter and test bounds
    v = dot(dir, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) {
        return -1.0;
	}

    // Calculate t, ray intersects triangle
    t = dot(edge2, qvec) * inv_det;

    return t;
}

// https://rootllama.wordpress.com/2014/05/26/point-in-polygon-test/
float isect_line(vec2 o, vec2 d, vec2 a, vec2 b ) {
    vec2 ortho = vec2( -d.y, d.x );
    vec2 aToO = vec2( o - a );
    vec2 aToB = vec2( b - a );
 
    float denom = dot( aToB, ortho );
 
    // Here would be a good time to see if denom is zero in which case the line segment and
    // the ray are parallel.
	if (denom == 0) {
		return -1;
	}
 
    float t1 = abs( aToB.x * aToO.y - aToO.x * aToB.y ) / denom;
    float t2 = dot( aToO, ortho ) / denom;
 
    if (t2 >= 0 && t2 <= 1 && t1 >= 0) {
		return t1;
	}
	return -1;
}

//==========================================================================================
// texture
//==========================================================================================

//https://en.wikipedia.org/wiki/Bilinear_filtering
float textureFilter(usampler2DArray tex, vec2 st, int layer) {
	ivec2 size = textureSize(tex, 0).xy;
	vec2 ratio = fract(st * size - 0.5);
	vec2 opposite = 1 - ratio;
	uvec4 value = textureGather(tex, vec3(st, layer));
	return
		(value.w * opposite.x + value.z * ratio.x) * opposite.y +
		(value.x * opposite.x + value.y * ratio.x) * ratio.y;
}
float textureFilter(usampler2DArray tex, vec2 st, int layer, uvec4 count) {
	ivec2 size = textureSize(tex, 0).xy;
	vec2 ratio = fract(st * size - 0.5);
	vec2 opposite = 1 - ratio;
	uvec4 x = textureGather(tex, vec3(st, layer));
	vec4 value = x / vec4(max(uvec4(1), count) * 255);
	return
		(value.w * opposite.x + value.z * ratio.x) * opposite.y +
		(value.x * opposite.x + value.y * ratio.x) * ratio.y;
}

//==========================================================================================
// hexagon
//==========================================================================================

vec2 hex_corner(vec2 center, float size, int i) {
	float angle_deg = 60 * i + 30;
	float angle_rad = radians(angle_deg);
	return vec2(
		center.x + size * cos(angle_rad),
		center.y + size * sin(angle_rad)
	);
}
vec2 hex_corner(vec2 center, int i) {
	return hex_corner(center, 1.0, i);
}

vec4 hex_edge(vec2 center, float size, int i) {
	return vec4(
		hex_corner(center, size, i),
		hex_corner(center, size, (i+1)%6)
	);
}
vec4 hex_edge(vec2 center, int i) {
	return hex_edge(center, 1.0, i);
}

vec3 hex_to_cube(vec2 hex) {
	return vec3(hex.x, hex.y, -hex.x - hex.y);
}
ivec3 hex_round(vec3 cube) {
	vec3 r = round(cube);
	vec3 diff = abs(r - cube);
	if (diff.x > diff.y && diff.x > diff.z) {
		r.x = -r.y - r.z;
	} else if (diff.y > diff.z) {
		r.y = -r.x - r.z;
	} else {
		r.z = -r.x - r.y;
	}
	return ivec3(r);
}
vec2 hex_round(vec2 hex) {
	vec3 cube = hex_to_cube(hex);
	ivec3 r = hex_round(cube);
	return vec2(r.xy);
}
vec2 pixel_to_hex(vec2 pixel) {
	vec2 hex = vec2(
		pixel.x * sqrt(3)/3 - pixel.y / 3,
		pixel.y * (2.0/3.0)
	);
	return hex;
}
vec2 hex_to_pixel(vec2 hex) {
	vec2 pixel = vec2(
		sqrt(3) * (hex.x + hex.y / 2),
		(3.0/2.0) * hex.y
	);
	return pixel;
}
vec2 hex_center(vec2 pixel, float size) {
	vec2 h = pixel_to_hex(pixel) / size;
	vec3 c = hex_to_cube(h);
	vec3 rc = hex_round(c);
	vec2 rh = rc.xy;
	vec2 hp = hex_to_pixel(rh) * size;
	return hp;
}
vec2 hex_center(vec2 pixel) {
	return hex_center(pixel, 1.0);
}

// { 2d cell id, distance to border, distance to center )
vec4 hexagon(vec2 p, float size) {
	vec2 h = pixel_to_hex(p) / size;
	vec3 c = hex_to_cube(h);
	vec3 rc = hex_round(c);
	vec2 rh = rc.xy;
	vec2 hp = hex_to_pixel(rh) * size;

	float dc = length(c - rc);
	float de = sdHex(p, hp, size, 0);
	
	return vec4(rh, de, dc);
}
vec4 hexagon(vec2 p) {
	return hexagon(p, 1);
}

const ivec2 HEX_DIRECTIONS[] = {
	ivec2(+1, 0), ivec2(+1, -1), ivec2(0, -1),
	ivec2(-1, 0), ivec2(-1, +1), ivec2(0, +1)
};
ivec2 hex_neighbor(ivec2 hex, int i) {
	ivec2 neighbor = hex + HEX_DIRECTIONS[i];
	return neighbor;
}
int hex_direction_reverse(int i) {
	return (i + 3) % 6;
}

vec3 rgb(float r, float g, float b) {
	return vec3(r, g, b) / 255;
}

vec3 pal(in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d) {
	return a + b*cos(2 * M_PI*(c*t + d));
}

float fill(float d, float k) {
    float a = 1.0 - smoothstep(0.0, k, d);
    return a;
}
float fill(float d) {
    return fill(d, 0.0015);
}

float grid_line(vec2 p) {
    float d = mincomp(abs(fract(p) - 0.5));
	return d*d;
}
float grid_point(vec2 p) {
	float d = maxcomp(abs(fract(p) - 0.5));
	return d;
}

float attenuation(float dist) {
	return 1 / (dist*dist);
}

/**
 * math & angles
 */
vec2 to_dir(float angle) {
	vec2 dir = vec2(
		cos(angle),
		sin(angle)
	);
	return dir;
}
float to_angle(vec2 dir) {
	float angle = atan(dir.y, dir.x);
	return angle;
}

mat4 mat4_scale(float x, float y, float z){
    return mat4(
        vec4(x,   0.0, 0.0, 0.0),
        vec4(0.0, y,   0.0, 0.0),
        vec4(0.0, 0.0, z,   0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 mat4_translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}
)
