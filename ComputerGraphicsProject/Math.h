#pragma once

// ---------------------------------------------------------------------------
// Computer Graphics - Core Mathematical Concepts (Math.h)
//
// Contents, in dependency order:
//   * maths constants / helpers  ................ slides 6 - 7
//   * Vec3 ...................................... slides 10 - 25
//   * Vec4 (homogeneous coords) ................. slides 26 - 28
//   * Matrix (4x4) .............................. slides 29 - 75
//   * Quaternion ................................ slides 96 - 114
//   * ShadingFrame .............................. slides 76 - 81
//   * Colour .................................... slides 115 - 117
//
// Quaternion convention used throughout this file (matches slide 97 and the
// toMatrix() code on slide 113):
//
//     q = d + a*i + b*j + c*k      ->  d is the REAL part
//                                      (a, b, c) is the imaginary/vector part
//
// i.e. the struct order is a, b, c, d (NOT w, x, y, z).
// ---------------------------------------------------------------------------

// _USE_MATH_DEFINES has to be defined BEFORE <cmath> is included and the name
// is case sensitive - the C runtime tests for exactly this spelling.
// (slide 6: "Need to #define _USE_MATH_DEFINES, before including any headers")
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <cstdio>
#include <algorithm>

// The lecture stores vectors as a nameless union of "float v[3]" and
// "struct { float x, y, z; }". That is a Microsoft extension, so silence the
// warning it produces instead of drowning it out at /W4.
#pragma warning(push)
#pragma warning(disable: 4201)

// Fallback in case the CRT does not hand out the M_* constants.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962
#endif
#ifndef M_1_PI
#define M_1_PI 0.31830988618379067154
#endif
#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

#define SQ(x) ((x) * (x))

template <typename T>
static T lerp(const T a, const T b, float t)
{
	return a * (1.0f - t) + (b * t);
}


// constructor for 3D vector
class Vec3
{
public:
	Vec3() : x(0), y(0), z(0) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	union
	{
		float v[3];
		struct { float x, y, z; };
	};

	// vector addition operator
	Vec3 operator +(const Vec3& pVec) const
	{
		return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
	}
	Vec3& operator +=(const Vec3& pVec)
	{
		v[0] += pVec.v[0];
		v[1] += pVec.v[1];
		v[2] += pVec.v[2];
		return *this;
	}
	// vector + scalar (added to every component)
	Vec3 operator +(const float val) const
	{
		return Vec3(v[0] + val, v[1] + val, v[2] + val);
	}
	Vec3& operator +=(const float val)
	{
		v[0] += val;
		v[1] += val;
		v[2] += val;
		return *this;
	}

	// vector multiplication operator
	Vec3 operator *(const Vec3& pVec) const
	{
		return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
	}
	Vec3 operator *(const float val) const
	{
		return Vec3(v[0] * val, v[1] * val, v[2] * val);
	}
	Vec3& operator *=(const Vec3& pVec)
	{
		v[0] *= pVec.v[0];
		v[1] *= pVec.v[1];
		v[2] *= pVec.v[2];
		return *this;
	}
	Vec3& operator *=(const float val)
	{
		v[0] *= val;
		v[1] *= val;
		v[2] *= val;
		return *this;
	}

	// vector minus operator
	Vec3 operator -(const Vec3& pVec) const
	{
		return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
	}
	Vec3 operator -(const float val) const
	{
		return Vec3(v[0] - val, v[1] - val, v[2] - val);
	}
	Vec3& operator -=(const Vec3& pVec)
	{
		v[0] -= pVec.v[0];
		v[1] -= pVec.v[1];
		v[2] -= pVec.v[2];
		return *this;
	}
	Vec3& operator -=(const float val)
	{
		v[0] -= val;
		v[1] -= val;
		v[2] -= val;
		return *this;
	}

	// unary negation (slide 14)
	Vec3 operator -() const
	{
		return Vec3(-v[0], -v[1], -v[2]);
	}

	// vector division operator
	Vec3 operator /(const Vec3& pVec) const
	{
		return Vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);
	}
	Vec3 operator /(const float val) const
	{
		return Vec3(v[0] / val, v[1] / val, v[2] / val);
	}
	Vec3& operator /=(const Vec3& pVec)
	{
		v[0] /= pVec.v[0];
		v[1] /= pVec.v[1];
		v[2] /= pVec.v[2];
		return *this;
	}
	Vec3& operator /=(const float val)
	{
		v[0] /= val;
		v[1] /= val;
		v[2] /= val;
		return *this;
	}

	// length / squared length (slide 15)
	float Length() const
	{
		return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
	float LengthSquare() const
	{
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}

	// vector normalization (returns a normalized copy)
	Vec3 normalize() const
	{
		const float length = Length();
		if (length == 0.0f)
			return Vec3(0.0f, 0.0f, 0.0f);	// avoid dividing by zero

		const float len = 1.0f / length;
		return Vec3(x * len, y * len, z * len);
	}
	// normalizes in place and returns the original length
	float normalize_GetLength()
	{
		float length = Length();
		if (length == 0.0f)
			return 0.0f;

		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}

	// dot product
	float Dot(const Vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}

	// cross product: this x v1
	// NOTE: the lecture slide writes this as
	//       return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1], ...)
	// which evaluates v1 x this (the operands are the other way round, so the
	// result is negated). The standard definition is used here so that
	// a.Cross(b) == a x b, i.e. x.Cross(y) == z.
	Vec3 Cross(const Vec3& v1) const
	{
		return Vec3(v[1] * v1.v[2] - v[2] * v1.v[1],
			v[2] * v1.v[0] - v[0] * v1.v[2],
			v[0] * v1.v[1] - v[1] * v1.v[0]);
	}

	// min/max of two vectors (static: does not use *this)
	static Vec3 Min(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(std::min(v1.v[0], v2.v[0]),
			std::min(v1.v[1], v2.v[1]),
			std::min(v1.v[2], v2.v[2]));
	}

	static Vec3 Max(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(std::max(v1.v[0], v2.v[0]),
			std::max(v1.v[1], v2.v[1]),
			std::max(v1.v[2], v2.v[2]));
	}

	// min/max components of a vector
	float Min() const
	{
		return std::min(x, std::min(y, z));
	}

	float Max() const
	{
		return std::max(x, std::max(y, z));
	}

	// print to console (slide 25)
	void Print(const char* label = "") const
	{
		printf("%s(%f, %f, %f)\n", label, x, y, z);
	}

};

// constructor for 4D vector (homogeneous coordinates, slide 26 - 28)
class Vec4
{
public:
	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	// homogeneous point from a 3D vector
	Vec4(const Vec3& p, float w = 1.0f) : x(p.x), y(p.y), z(p.z), w(w) {}

	union
	{
		float v[4];
		struct { float x, y, z, w; };
	};

	Vec4 operator +(const Vec4& pVec) const
	{
		return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
	}
	Vec4& operator +=(const Vec4& pVec)
	{
		v[0] += pVec.v[0]; v[1] += pVec.v[1]; v[2] += pVec.v[2]; v[3] += pVec.v[3];
		return *this;
	}
	Vec4 operator -(const Vec4& pVec) const
	{
		return Vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], v[3] - pVec.v[3]);
	}
	Vec4& operator -=(const Vec4& pVec)
	{
		v[0] -= pVec.v[0]; v[1] -= pVec.v[1]; v[2] -= pVec.v[2]; v[3] -= pVec.v[3];
		return *this;
	}
	// component-wise product (colour-like use later on)
	Vec4 operator *(const Vec4& pVec) const
	{
		return Vec4(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2], v[3] * pVec.v[3]);
	}
	Vec4 operator *(const float val) const
	{
		return Vec4(v[0] * val, v[1] * val, v[2] * val, v[3] * val);
	}
	Vec4& operator *=(const float val)
	{
		v[0] *= val; v[1] *= val; v[2] *= val; v[3] *= val;
		return *this;
	}
	Vec4 operator /(const float val) const
	{
		return Vec4(v[0] / val, v[1] / val, v[2] / val, v[3] / val);
	}
	Vec4& operator /=(const float val)
	{
		v[0] /= val; v[1] /= val; v[2] /= val; v[3] /= val;
		return *this;
	}
	Vec4 operator -() const
	{
		return Vec4(-v[0], -v[1], -v[2], -v[3]);
	}

	float Dot(const Vec4& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + v[3] * pVec.v[3];
	}

	// drop w (perspective divide, if w != 0) -> used a lot from now on
	Vec3 xyz() const
	{
		return Vec3(x, y, z);
	}
	Vec3 perspectiveDivide() const
	{
		if (w == 0.0f || w == 1.0f)
			return Vec3(x, y, z);
		return Vec3(x / w, y / w, z / w);
	}

	void Print(const char* label = "") const
	{
		printf("%s(%f, %f, %f, %f)\n", label, x, y, z, w);
	}
};

// constructor for 4x4 matrix (row major - slide 62)
class Matrix
{
public:
	// default constructor initializes to the identity (slide 38)
	Matrix()
	{
		identity();
	}

	// explicit initialization from 16 values (row major order)
	explicit Matrix(const float* values)
	{
		for (int i = 0; i < 16; i++)
			m[i] = values[i];
	}

	union
	{
		float a[4][4];
		float m[16];
	};

	// no transformation (slide 33)
	void identity()
	{
		for (int i = 0; i < 16; i++)
			m[i] = 0.0f;
		m[0] = m[5] = m[10] = m[15] = 1.0f;
	}

	// access matrix elements using array indexing
	float& operator[] (int index)
	{
		return m[index];
	}
	float operator[] (int index) const
	{
		return m[index];
	}

	// transpose the matrix (flips rows and columns, slide 61)
	void transpose()
	{
		float temp = a[0][1]; a[0][1] = a[1][0]; a[1][0] = temp;
		temp = a[0][2]; a[0][2] = a[2][0]; a[2][0] = temp;
		temp = a[0][3]; a[0][3] = a[3][0]; a[3][0] = temp;
		temp = a[1][2]; a[1][2] = a[2][1]; a[2][1] = temp;
		temp = a[1][3]; a[1][3] = a[3][1]; a[3][1] = temp;
		temp = a[2][3]; a[2][3] = a[3][2]; a[3][2] = temp;
	}

	// transposed copy
	Matrix transposed() const
	{
		Matrix ret = *this;
		ret.transpose();
		return ret;
	}

	// rotate around the X axis
	void rotateX(float angle)
	{
		const float cosine = cosf(angle);
		const float sine = sinf(angle);
		m[0] = 1.0f; m[1] = 0.0f;   m[2] = 0.0f;    m[3] = 0.0f;
		m[4] = 0.0f; m[5] = cosine; m[6] = -sine;  m[7] = 0.0f;
		m[8] = 0.0f; m[9] = sine;   m[10] = cosine; m[11] = 0.0f;
		m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
	}

	// rotate around the Y axis
	void rotateY(float angle)
	{
		const float cosine = cosf(angle);
		const float sine = sinf(angle);
		m[0] = cosine; m[1] = 0.0f; m[2] = sine;   m[3] = 0.0f;
		m[4] = 0.0f;   m[5] = 1.0f; m[6] = 0.0f;   m[7] = 0.0f;
		m[8] = -sine;  m[9] = 0.0f; m[10] = cosine; m[11] = 0.0f;
		m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
	}

	// rotate around the Z axis
	void rotateZ(float angle)
	{
		const float cosine = cosf(angle);
		const float sine = sinf(angle);
		m[0] = cosine; m[1] = -sine;  m[2] = 0.0f; m[3] = 0.0f;
		m[4] = sine;   m[5] = cosine; m[6] = 0.0f; m[7] = 0.0f;
		m[8] = 0.0f;  m[9] = 0.0f;   m[10] = 1.0f; m[11] = 0.0f;
		m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
	}

	// translation (slide 35 / 49). The translation lives in the 4th column,
	// which is what mulPoint() adds.
	void setTranslate(const Vec3& t)
	{
		identity();
		m[3] = t.x; m[7] = t.y; m[11] = t.z;
	}
	// add a translation to an existing transformation
	void translate(const Vec3& t)
	{
		m[3] += t.x; m[7] += t.y; m[11] += t.z;
	}
	Vec3 getTranslate() const
	{
		return Vec3(m[3], m[7], m[11]);
	}

	// scaling (slide 37 / 49)
	void setScale(const Vec3& s)
	{
		identity();
		m[0] = s.x; m[5] = s.y; m[10] = s.z;
	}
	void setScale(float s)
	{
		setScale(Vec3(s, s, s));
	}

	// multiply a 4D vector by the matrix
	Vec4 operator*(const Vec4& v) const
	{
		return Vec4(
			v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3],
			v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7],
			v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11],
			v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]);
	}

	// multiply a 3D point by the matrix (including translation)
	Vec3 mulPoint(const Vec3& v) const
	{
		return Vec3(
			v.x * m[0] + v.y * m[1] + v.z * m[2] + m[3],
			v.x * m[4] + v.y * m[5] + v.z * m[6] + m[7],
			v.x * m[8] + v.y * m[9] + v.z * m[10] + m[11]);
	}

	// multiply a 3D vector by the matrix (ignoring translation)
	Vec3 mulVec(const Vec3& v) const
	{
		return Vec3(
			v.x * m[0] + v.y * m[1] + v.z * m[2],
			v.x * m[4] + v.y * m[5] + v.z * m[6],
			v.x * m[8] + v.y * m[9] + v.z * m[10]);
	}

	// multiply two matrices
	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
		ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
		ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
		ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
		ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
		ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
		ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
		ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
		ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
		ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
		ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
		ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
		ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
		ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
		ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
		ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
		return ret;
	}

	// multiply two matrices operator for natural access
	Matrix operator* (const Matrix& matrix) const
	{
		return mul(matrix);
	}

	// invert the matrix (closed form adjugate, slide 75)
	Matrix invert() const
	{
		Matrix inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		// Singular matrix: there is no inverse. Returning the identity is the
		// usual "safe" fallback (the alternative is to assert and fail loudly).
		if (fabsf(det) < 1e-8f)
			return Matrix();

		det = 1.0f / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}

	// determinant, via the cofactor expansion along the first row (slide 63-67)
	float determinant() const
	{
		return
			m[0] * (m[5] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[6] * m[15] - m[7] * m[14]) + m[13] * (m[6] * m[11] - m[7] * m[10])) -
			m[1] * (m[4] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[6] * m[15] - m[7] * m[14]) + m[12] * (m[6] * m[11] - m[7] * m[10])) +
			m[2] * (m[4] * (m[9] * m[15] - m[11] * m[13]) - m[8] * (m[5] * m[15] - m[7] * m[13]) + m[12] * (m[5] * m[11] - m[7] * m[9])) -
			m[3] * (m[4] * (m[9] * m[14] - m[10] * m[13]) - m[8] * (m[5] * m[14] - m[6] * m[13]) + m[12] * (m[5] * m[10] - m[6] * m[9]));
	}

	void Print(const char* label = "") const
	{
		printf("%s\n", label);
		for (int row = 0; row < 4; row++)
			printf("  [%9.5f %9.5f %9.5f %9.5f]\n",
				m[row * 4 + 0], m[row * 4 + 1], m[row * 4 + 2], m[row * 4 + 3]);
	}
};


// ---------------------------------------------------------------------------
// Quaternion (slides 96 - 114)
//
//     q = d + a*i + b*j + c*k
//
// A unit quaternion represents a rotation *by theta about an axis*:
//     q = cos(theta/2) + sin(theta/2) * (axis.x i + axis.y j + axis.z k)   (slide 102)
//
// Rotation of a point:   p' = q * p_q * q^-1   with p_q = 0 + p.x i + p.y j + p.z k
// ---------------------------------------------------------------------------
class Quaternion
{
public:
	// struct order matters - it is the order used in the lecture (slide 97)
	union
	{
		struct { float a, b, c, d; };
		float q[4];
	};

	// identity rotation: 1 + 0i + 0j + 0k
	Quaternion() : a(0.0f), b(0.0f), c(0.0f), d(1.0f) {}
	// q = d + a i + b j + c k
	Quaternion(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}

	// from an axis and an angle in RADIANS (slide 102)
	//     q = cos(theta/2) + sin(theta/2) * (vx i + vy j + vz k)
	static Quaternion FromAxisAngle(const Vec3& axis, float angle)
	{
		const Vec3 n = axis.normalize();			// the axis must be unit length
		const float halfAngle = angle * 0.5f;
		const float s = sinf(halfAngle);
		const float c = cosf(halfAngle);
		return Quaternion(n.x * s, n.y * s, n.z * s, c);
	}

	// magnitude |q| = sqrt(a^2 + b^2 + c^2 + d^2)  (slide 100)
	float Magnitude() const
	{
		return sqrtf(a * a + b * b + c * c + d * d);
	}

	// normalization q / |q| (slide 100)
	Quaternion Normalize() const
	{
		const float length = Magnitude();
		if (length == 0.0f)
			return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

		const float len = 1.0f / length;
		return Quaternion(a * len, b * len, c * len, d * len);
	}

	// conjugate q* = d - a i - b j - c k (slide 100)
	Quaternion Conjugate() const
	{
		return Quaternion(-a, -b, -c, d);
	}

	// inverse q^-1 = q* / |q|^2   (slide 100 writes q* / |q|, which is only
	// correct for unit quaternions; the squared magnitude is the general form)
	Quaternion Inverse() const
	{
		const float lenSq = a * a + b * b + c * c + d * d;
		if (lenSq == 0.0f)
			return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

		const float invLenSq = 1.0f / lenSq;
		return Quaternion(-a * invLenSq, -b * invLenSq, -c * invLenSq, d * invLenSq);
	}

	// quaternion dot product q1.q2 = d1d2 + a1a2 + b1b2 + c1c2 (slide 106)
	float Dot(const Quaternion& q2) const
	{
		return a * q2.a + b * q2.b + c * q2.c + d * q2.d;
	}

	// addition / subtraction (component wise - needed by Slerp)
	Quaternion operator +(const Quaternion& quat) const
	{
		return Quaternion(a + quat.a, b + quat.b, c + quat.c, d + quat.d);
	}
	Quaternion operator -(const Quaternion& quat) const
	{
		return Quaternion(a - quat.a, b - quat.b, c - quat.c, d - quat.d);
	}
	Quaternion operator -() const
	{
		return Quaternion(-a, -b, -c, -d);
	}

	// scalar multiply / divide
	Quaternion operator *(const float val) const
	{
		return Quaternion(a * val, b * val, c * val, d * val);
	}
	Quaternion operator /(const float val) const
	{
		return Quaternion(a / val, b / val, c / val, d / val);
	}
	Quaternion& operator *=(const float val)
	{
		a *= val; b *= val; c *= val; d *= val;
		return *this;
	}

	// Quaternion multiplication (Hamilton product, slide 101)
	//     q1 x q2 = (d1d2 - a1a2 - b1b2 - c1c2)
	//             + (d1a2 + a1d2 + b1c2 - c1b2) i
	//             + (d1b2 - a1c2 + b1d2 + c1a2) j
	//             + (d1c2 + a1b2 - b1a2 + c1d2) k
	// Remember the order: (qA * qB) applies qB first, then qA.
	Quaternion operator *(const Quaternion& q2) const
	{
		return Quaternion(
			d * q2.a + a * q2.d + b * q2.c - c * q2.b,		// i
			d * q2.b - a * q2.c + b * q2.d + c * q2.a,		// j
			d * q2.c + a * q2.b - b * q2.a + c * q2.d,		// k
			d * q2.d - a * q2.a - b * q2.b - c * q2.c);		// real
	}
	Quaternion& operator *=(const Quaternion& q2)
	{
		*this = *this * q2;
		return *this;
	}

	// Slerp: spherical linear interpolation (slides 103 - 110)
	//   q(t) = sin(theta(1-t)) / sin(theta) * q1 + sin(theta t) / sin(theta) * q2
	//   theta = acos(q1 . q2)
	// q1 and q2 should be unit quaternions.
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		float cosTheta = q1.Dot(q2);

		// q and -q describe the same rotation: if the dot product is negative
		// the interpolation would take the long way round, so flip q2.
		// (slide 110)
		Quaternion end = q2;
		if (cosTheta < 0.0f)
		{
			end = -q2;
			cosTheta = -cosTheta;
		}

		// Very close (or identical) rotations: sin(theta) -> 0 and the
		// formula blows up, so fall back to a normalized lerp.
		if (cosTheta > 0.9995f)
			return (q1 + (end - q1) * t).Normalize();

		// guard against floating point error pushing the cosine outside [-1, 1]
		cosTheta = std::min(1.0f, std::max(-1.0f, cosTheta));

		const float theta = acosf(cosTheta);
		const float sinTheta = sinf(theta);
		const float w1 = sinf((1.0f - t) * theta) / sinTheta;
		const float w2 = sinf(t * theta) / sinTheta;

		return q1 * w1 + end * w2;
	}
	// convenient member version: interpolate from *this to q2
	Quaternion Slerp(const Quaternion& q2, float t) const
	{
		return Slerp(*this, q2, t);
	}

	// rotate a point/vector: p' = q * p_q * q^-1 (slide 111)
	Vec3 Rotate(const Vec3& p) const
	{
		const Quaternion pq(p.x, p.y, p.z, 0.0f);	// pure quaternion
		const Quaternion result = (*this) * pq * Inverse();
		return Vec3(result.a, result.b, result.c);
	}

	// matrix form of the quaternion (slide 112 - 113)
	// The closed form below assumes a unit quaternion, so normalise first -
	// otherwise the "rotation" matrix would not be orthonormal.
	Matrix toMatrix() const
	{
		float len = Magnitude();
		len = (len > 0.0f) ? (1.0f / len) : 1.0f;

		const float na = a * len;
		const float nb = b * len;
		const float nc = c * len;
		const float nd = d * len;

		const float aa = na * na, ab = na * nb, ac = na * nc;
		const float bb = nb * nb, bc = nb * nc, cc = nc * nc;
		const float da = nd * na, db = nd * nb, dc = nd * nc;

		Matrix m;
		m[0] = 1.0f - 2.0f * (bb + cc); m[1] = 2.0f * (ab - dc);		m[2] = 2.0f * (ac + db);		m[3] = 0.0f;
		m[4] = 2.0f * (ab + dc);		m[5] = 1.0f - 2.0f * (aa + cc); m[6] = 2.0f * (bc - da);		m[7] = 0.0f;
		m[8] = 2.0f * (ac - db);		m[9] = 2.0f * (bc + da);		m[10] = 1.0f - 2.0f * (aa + bb); m[11] = 0.0f;
		m[12] = 0.0f;				m[13] = 0.0f;					m[14] = 0.0f;					m[15] = 1.0f;
		return m;
	}

	void Print(const char* label = "") const
	{
		printf("%sa=%f b=%f c=%f d=%f  (|q|=%f)\n", label, a, b, c, d, Magnitude());
	}
};


// ---------------------------------------------------------------------------
// Shading Frame (slides 76 - 81)
//
// An orthonormal basis {u, v, n} with n = z axis of shading space.
// The same thing as the rotation matrix
//
//     F = [ ux  vx  nx ]
//         [ uy  vy  ny ]
//         [ uz  vz  nz ]
//
// which rotates SHADING space into WORLD space. Because F is orthonormal its
// inverse is simply its transpose, so world -> shading is the transpose.
// ---------------------------------------------------------------------------
class ShadingFrame
{
public:
	Vec3 u;		// shading x axis
	Vec3 v;		// shading y axis
	Vec3 n;		// shading z axis (the "up" vector / normal)

	// identity frame
	ShadingFrame() : u(1.0f, 0.0f, 0.0f), v(0.0f, 1.0f, 0.0f), n(0.0f, 0.0f, 1.0f) {}

	// build from a single normal using Gram-Schmidt (slides 78 - 80)
	explicit ShadingFrame(const Vec3& normal)
	{
		Build(normal);
	}

	// build from a TBN (slide 76), orthonormalised with Gram-Schmidt
	ShadingFrame(const Vec3& tangent, const Vec3& bitangent, const Vec3& normal)
	{
		Build(tangent, bitangent, normal);
	}

	// Gram-Schmidt from one vector (slides 79 - 80):
	//   1. pick an axis u' that is not colinear with n
	//   2. u = normalize(n x u')
	//   3. v = n x u        (which makes u x v = n, i.e. right handed)
	void Build(const Vec3& normal)
	{
		n = normal.normalize();

		// (1,0,0) unless n is (almost) parallel to it, then (0,1,0)
		const Vec3 uPrime = (fabsf(n.x) < 0.9999f) ? Vec3(1.0f, 0.0f, 0.0f)
			: Vec3(0.0f, 1.0f, 0.0f);

		u = n.Cross(uPrime).normalize();
		v = n.Cross(u);
	}

	// Gram-Schmidt from a tangent + normal, keeping the handedness of the
	// supplied bitangent (this is what a normal map needs).
	void Build(const Vec3& tangent, const Vec3& bitangent, const Vec3& normal)
	{
		n = normal.normalize();

		// remove the component of the tangent that lies along n
		const Vec3 t = tangent - n * n.Dot(tangent);

		// tangent parallel to the normal: nothing to work with, fall back
		if (t.LengthSquare() < 1e-8f)
		{
			Build(normal);
			return;
		}

		u = t.normalize();
		v = n.Cross(u);

		// keep the winding/handedness of the input frame
		if (v.Dot(bitangent) < 0.0f)
			v = -v;
	}

	// the rotation matrix F (shading -> world), columns are u, v, n (slide 78)
	Matrix toMatrix() const
	{
		Matrix m;
		m[0] = u.x; m[1] = v.x; m[2] = n.x; m[3] = 0.0f;
		m[4] = u.y; m[5] = v.y; m[6] = n.y; m[7] = 0.0f;
		m[8] = u.z; m[9] = v.z; m[10] = n.z; m[11] = 0.0f;
		m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
		return m;
	}

	// inverse of the frame: the transpose is the inverse for an orthonormal
	// matrix (slide 81), so this is world -> shading
	Matrix toMatrixInverse() const
	{
		Matrix m = toMatrix();
		m.transpose();
		return m;
	}

	// forward transform: shading space -> world space
	Vec3 ToWorld(const Vec3& shading) const
	{
		return u * shading.x + v * shading.y + n * shading.z;
	}

	// inverse transform: world space -> shading space
	Vec3 ToShading(const Vec3& world) const
	{
		return Vec3(world.Dot(u), world.Dot(v), world.Dot(n));
	}

	bool IsOrthonormal(float epsilon = 1e-4f) const
	{
		return fabsf(u.LengthSquare() - 1.0f) < epsilon &&
			fabsf(v.LengthSquare() - 1.0f) < epsilon &&
			fabsf(n.LengthSquare() - 1.0f) < epsilon &&
			fabsf(u.Dot(v)) < epsilon &&
			fabsf(u.Dot(n)) < epsilon &&
			fabsf(v.Dot(n)) < epsilon;
	}

	void Print(const char* label = "") const
	{
		printf("%su", label); u.Print(" = ");
		printf("%sv", label); v.Print(" = ");
		printf("%sn", label); n.Print(" = ");
	}
};


// ---------------------------------------------------------------------------
// Colour (slides 115 - 117)
//
// Stored as linear RGB + alpha in [0, 1].
// ---------------------------------------------------------------------------
class Colour
{
public:
	union
	{
		float c[4];
		struct { float r, g, b, a; };
	};

	// black, opaque
	Colour() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	// from floats in [0, 1]
	Colour(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
	// from 8 bit values, e.g. image data: r = input_r / 255.0f (slide 116).
	// The lecture writes this as a constructor taking unsigned char; taking an
	// int instead keeps Colour(255, 128, 0) unambiguous (with an unsigned char
	// overload, integer literals would not know which constructor to pick).
	Colour(int r, int g, int b, int a = 255)
		: r(std::min(255, std::max(0, r)) / 255.0f),
		  g(std::min(255, std::max(0, g)) / 255.0f),
		  b(std::min(255, std::max(0, b)) / 255.0f),
		  a(std::min(255, std::max(0, a)) / 255.0f) {}
	// from a colour + explicit alpha
	Colour(const Colour& col, float alpha) : r(col.r), g(col.g), b(col.b), a(alpha) {}

	Colour operator +(const Colour& colour) const
	{
		return Colour(r + colour.r, g + colour.g, b + colour.b, a + colour.a);
	}
	Colour& operator +=(const Colour& colour)
	{
		r += colour.r; g += colour.g; b += colour.b; a += colour.a;
		return *this;
	}
	Colour operator -(const Colour& colour) const
	{
		return Colour(r - colour.r, g - colour.g, b - colour.b, a - colour.a);
	}
	Colour& operator -=(const Colour& colour)
	{
		r -= colour.r; g -= colour.g; b -= colour.b; a -= colour.a;
		return *this;
	}
	// component wise product (modulation, e.g. albedo * light colour)
	Colour operator *(const Colour& colour) const
	{
		return Colour(r * colour.r, g * colour.g, b * colour.b, a * colour.a);
	}
	Colour& operator *=(const Colour& colour)
	{
		r *= colour.r; g *= colour.g; b *= colour.b; a *= colour.a;
		return *this;
	}
	Colour operator *(const float val) const
	{
		return Colour(r * val, g * val, b * val, a);
	}
	Colour& operator *=(const float val)
	{
		r *= val; g *= val; b *= val;
		return *this;
	}
	Colour operator /(const float val) const
	{
		return Colour(r / val, g / val, b / val, a);
	}
	Colour& operator /=(const float val)
	{
		r /= val; g /= val; b /= val;
		return *this;
	}

	// clamp every channel into [0, 1] - handy before writing to an image
	Colour Clamp() const
	{
		return Colour(std::min(1.0f, std::max(0.0f, r)),
			std::min(1.0f, std::max(0.0f, g)),
			std::min(1.0f, std::max(0.0f, b)),
			std::min(1.0f, std::max(0.0f, a)));
	}

	// scalar luminance (Rec. 709 weights) - useful for greyscale
	float Luminance() const
	{
		return 0.2126f * r + 0.7152f * g + 0.0722f * b;
	}

	void Print(const char* label = "") const
	{
		printf("%s(%f, %f, %f, %f)\n", label, r, g, b, a);
	}
};

// allow "float * colour" as well as "colour * float"
inline Colour operator *(const float val, const Colour& colour)
{
	return colour * val;
}

// "float * Vec3" for symmetry with the colour overload above
inline Vec3 operator *(const float val, const Vec3& vec)
{
	return vec * val;
}

#pragma warning(pop)
