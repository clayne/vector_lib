/* types.h  -  Vector library  -  Public Domain  -  2013 Mattias Jansson
 *
 * This library provides a cross-platform vector math library in C11 providing basic support data
 * types and functions to write applications and games in a platform-independent fashion. The latest
 * source code is always available at
 *
 * https://github.com/mjansson/vector_lib
 *
 * This library is built on top of the foundation library available at
 *
 * https://github.com/mjansson/foundation_lib
 *
 * This library is put in the public domain; you can redistribute it and/or modify it without any
 * restrictions.
 *
 */

#pragma once

/*! \file types.h
    Vector math types */

#include <foundation/platform.h>
#include <foundation/math.h>
#include <foundation/assert.h>

#include <vector/build.h>

#if VECTOR_IMPLEMENTATION_SSE4 || VECTOR_IMPLEMENTATION_SSE3 || VECTOR_IMPLEMENTATION_SSE2

#include <emmintrin.h>

#define VECTOR_ALIGN FOUNDATION_ALIGN(16)
#define VECTOR_ALIGNED_STRUCT(s) FOUNDATION_ALIGNED_STRUCT(s, 16)

#if FOUNDATION_COMPILER_GCC || FOUNDATION_COMPILER_CLANG
typedef __m128 vector_t VECTOR_ALIGN;
typedef __m128i vectori_t VECTOR_ALIGN;
typedef float vector_arr_t[4] VECTOR_ALIGN;
#else
typedef VECTOR_ALIGN __m128 vector_t;
typedef VECTOR_ALIGN __m128i vectori_t;
typedef VECTOR_ALIGN float vector_arr_t[4];
#endif

#if VECTOR_IMPLEMENTATION_SSE4 || VECTOR_IMPLEMENTATION_SSE3
#include <pmmintrin.h>
#endif

#else

#define VECTOR_ALIGN
#define VECTOR_ALIGNED_STRUCT(s) struct s

typedef struct vector_t vector_t;
typedef struct vectori_t vectori_t;
typedef float vector_arr_t[4];

struct vector_t {
	float32_t x;
	float32_t y;
	float32_t z;
	float32_t w;
};

struct vectori_t {
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t w;
};

#endif

typedef union matrix_t matrix_t;

//! Row major matrix where matrix row elements reside next to each other in memory.
union matrix_t {
	//! Component access
	VECTOR_ALIGNED_STRUCT(matrix_component_t) {
		float32_t m00, m01, m02, m03;  // Row 0
		float32_t m10, m11, m12, m13;  // Row 1
		float32_t m20, m21, m22, m23;  // Row 2
		float32_t m30, m31, m32, m33;  // Row 3
	}
	comp;
	//! Array access, flat layout
	VECTOR_ALIGN float32_t arr[16];
	//! 2-dimensional row access, each row in one major array, frow[row_index][column_index]
	VECTOR_ALIGN float32_t frow[4][4];
	//! Vector access, each row is one vector, row[row_index]
	vector_t row[4];
};

typedef vector_t quaternion_t;
typedef vector_t euler_angles_t;  // Order as uint32_t in w component

typedef struct dual_quaternion_t dual_quaternion_t;
typedef struct transform_t transform_t;
typedef struct vector_config_t vector_config_t;

VECTOR_ALIGNED_STRUCT(dual_quaternion_t) {
	quaternion_t q[2];
};

VECTOR_ALIGNED_STRUCT(transform_t) {
	quaternion_t rotation;
	vector_t translation;  // Scale in w component
};

#define VECTOR_GETEULERORDER(i, p, r, f) ((((((i << 1) + p) << 1) + r) << 1) + f)

#define VECTOR_EULER_STATICFRAME 0
#define VECTOR_EULER_ROTATEFRAME 1
#define VECTOR_EULER_NOREPEAT 0
#define VECTOR_EULER_REPEAT 1
#define VECTOR_EULER_EVEN 0
#define VECTOR_EULER_ODD 1

/*! \brief Order identifiers

    Order identifiers describing the order and method of which the rotations
    are applied. The three upper-case letters denote the axes on which the rotations
    are applied, and the lower-case letter denote if it is a static frame or rotating
    frame of reference (if each consecutive rotation are applied in the unrotated
    coordinate system - 's', or in the rotated coordinate system - 'r')

    The default order is XYZs */
typedef enum euler_angles_order_t {
	EULER_DEFAULTORDER = VECTOR_GETEULERORDER(0, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),

	EULER_XYZs = VECTOR_GETEULERORDER(0, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),
	EULER_XYXs = VECTOR_GETEULERORDER(0, VECTOR_EULER_EVEN, VECTOR_EULER_REPEAT, VECTOR_EULER_STATICFRAME),
	EULER_XZYs = VECTOR_GETEULERORDER(0, VECTOR_EULER_ODD, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),
	EULER_XZXs = VECTOR_GETEULERORDER(0, VECTOR_EULER_ODD, VECTOR_EULER_REPEAT, VECTOR_EULER_STATICFRAME),
	EULER_YZXs = VECTOR_GETEULERORDER(1, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),
	EULER_YZYs = VECTOR_GETEULERORDER(1, VECTOR_EULER_EVEN, VECTOR_EULER_REPEAT, VECTOR_EULER_STATICFRAME),
	EULER_YXZs = VECTOR_GETEULERORDER(1, VECTOR_EULER_ODD, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),
	EULER_YXYs = VECTOR_GETEULERORDER(1, VECTOR_EULER_ODD, VECTOR_EULER_REPEAT, VECTOR_EULER_STATICFRAME),
	EULER_ZXYs = VECTOR_GETEULERORDER(2, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),
	EULER_ZXZs = VECTOR_GETEULERORDER(2, VECTOR_EULER_EVEN, VECTOR_EULER_REPEAT, VECTOR_EULER_STATICFRAME),
	EULER_ZYXs = VECTOR_GETEULERORDER(2, VECTOR_EULER_ODD, VECTOR_EULER_NOREPEAT, VECTOR_EULER_STATICFRAME),
	EULER_ZYZs = VECTOR_GETEULERORDER(2, VECTOR_EULER_ODD, VECTOR_EULER_REPEAT, VECTOR_EULER_STATICFRAME),

	EULER_ZYXr = VECTOR_GETEULERORDER(0, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_XYXr = VECTOR_GETEULERORDER(0, VECTOR_EULER_EVEN, VECTOR_EULER_REPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_YZXr = VECTOR_GETEULERORDER(0, VECTOR_EULER_ODD, VECTOR_EULER_NOREPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_XZXr = VECTOR_GETEULERORDER(0, VECTOR_EULER_ODD, VECTOR_EULER_REPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_XZYr = VECTOR_GETEULERORDER(1, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_YZYr = VECTOR_GETEULERORDER(1, VECTOR_EULER_EVEN, VECTOR_EULER_REPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_ZXYr = VECTOR_GETEULERORDER(1, VECTOR_EULER_ODD, VECTOR_EULER_NOREPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_YXYr = VECTOR_GETEULERORDER(1, VECTOR_EULER_ODD, VECTOR_EULER_REPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_YXZr = VECTOR_GETEULERORDER(2, VECTOR_EULER_EVEN, VECTOR_EULER_NOREPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_ZXZr = VECTOR_GETEULERORDER(2, VECTOR_EULER_EVEN, VECTOR_EULER_REPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_XYZr = VECTOR_GETEULERORDER(2, VECTOR_EULER_ODD, VECTOR_EULER_NOREPEAT, VECTOR_EULER_ROTATEFRAME),
	EULER_ZYZr = VECTOR_GETEULERORDER(2, VECTOR_EULER_ODD, VECTOR_EULER_REPEAT, VECTOR_EULER_ROTATEFRAME)
} euler_angles_order_t;

typedef VECTOR_ALIGN float32_t float32_aligned128_t;

FOUNDATION_STATIC_ASSERT(sizeof(vector_t) == sizeof(float32_t) * 4, "vector size");
FOUNDATION_STATIC_ASSERT(sizeof(matrix_t) == sizeof(float32_t) * 16, "matrix size");
FOUNDATION_STATIC_ASSERT(sizeof(transform_t) == sizeof(float32_t) * 8, "transform size");
FOUNDATION_STATIC_ASSERT(sizeof(euler_angles_t) == sizeof(float32_t) * 4, "euler angles size");

struct vector_config_t {
	int unused;
};
