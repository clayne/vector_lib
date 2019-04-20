/* quaternion.h  -  Vector library  -  Public Domain  -  2013 Mattias Jansson / Rampant Pixels
 *
 * This library provides a cross-platform vector math library in C11 providing basic support data types and
 * functions to write applications and games in a platform-independent fashion. The latest source code is
 * always available at
 *
 * https://github.com/rampantpixels/vector_lib
 *
 * This library is built on top of the foundation library available at
 *
 * https://github.com/rampantpixels/foundation_lib
 *
 * This library is put in the public domain; you can redistribute it and/or modify it without any
 * restrictions.
 *
 */

#pragma once

/*! \file quaternion.h
Rotation abstraction using quaternions */

#include <vector/types.h>
#include <vector/mask.h>
#include <vector/vector.h>

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_zero(void);

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_identity(void);

//! Load scalars
static FOUNDATION_FORCEINLINE FOUNDATION_PURECALL quaternion_t
quaternion_scalar(float32_t x, float32_t y, float32_t z, float32_t w);

//! Load unaligned
static FOUNDATION_FORCEINLINE FOUNDATION_PURECALL quaternion_t
quaternion_unaligned(const float32_t* FOUNDATION_RESTRICT q);

//! Load aligned (16-byte alignment)
static FOUNDATION_FORCEINLINE FOUNDATION_PURECALL quaternion_t
quaternion_aligned(const float32_aligned128_t* FOUNDATION_RESTRICT q);

// Quaternion conjugate, negating vector component of quaterinon, q' = (-q.x, -q.y, -q.z, q.w)
static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_conjugate(const quaternion_t q);

// Quaternion inverse. For a unit quaternion equivalent to conjugate.
// in = 1 / vector_length_sqr(q)
// q' = (q.x * -in, q.y * -in, q.z * -in, q.w * in)
static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_inverse(const quaternion_t q);

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_neg(const quaternion_t q);

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_normalize(const quaternion_t q);

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_mul(const quaternion_t q0, const quaternion_t q1);

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_add(const quaternion_t q0, const quaternion_t q1);

static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_sub(const quaternion_t q0, const quaternion_t q1);

// Quaternions must be unit length
static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL quaternion_t
quaternion_slerp(const quaternion_t q0, const quaternion_t q1, real factor);

// Vector is treated as directional vector [x, y, z, 0] and returns
// a directional vector [x', y', z', 0]
static FOUNDATION_FORCEINLINE FOUNDATION_CONSTCALL vector_t
quaternion_rotate(const quaternion_t q, const vector_t v);

#if VECTOR_IMPLEMENTATION_SSE4
#include <vector/quaternion_sse4.h>
#elif VECTOR_IMPLEMENTATION_SSE3
#include <vector/quaternion_sse3.h>
#elif VECTOR_IMPLEMENTATION_SSE2
#include <vector/quaternion_sse2.h>
#elif VECTOR_IMPLEMENTATION_NEON
#include <vector/quaternion_neon.h>
#else
#include <vector/quaternion_fallback.h>
#endif
