#pragma once
#include <cmath>

#define _UseMathDefines
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

	// vector normalization
	Vec3 normalize()
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return Vec3(x * len, y * len, z * len);
	}
	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}

	// dot product
	float Dot(const Vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}

	// cross product
	Vec3 Cross(const Vec3& v1)
	{
		return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
			v1.v[2] * v[0] - v1.v[0] * v[2],
			v1.v[0] * v[1] - v1.v[1] * v[0]);
	}

	// min/max of two vectors
	Vec3 Min(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(min(v1.v[0], v2.v[0]),
			min(v1.v[1], v2.v[1]),
			min(v1.v[2], v2.v[2]));
	}
	
	Vec3 Max(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(max(v1.v[0], v2.v[0]),
			max(v1.v[1], v2.v[1]),
			max(v1.v[2], v2.v[2]));
	}
	
	// min/max components of a vector
	float Min() const
	{
		return min(x, min(y, z));
	}
	
	float Max() const
	{
		return max(x, max(y, z));
	}

};

// constructor for 4D vector
class Vec4
{
public:
	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	union
	{
		float v[4];
		struct { float x, y, z, w; };
	};

};

// constructor for 4x4 matrix
class Matrix 
{
public:
	union 
	{
		float a[4][4];
		float m[16];
	};

	// access matrix elements using array indexing
	float& operator[] (int index)
	{
		return m[index];
	}

	// transpose the matrix (flips rows and columns)
	void transpose()
	{
		float temp = a[0][1]; a[0][1] = a[1][0]; a[1][0] = temp;
		temp = a[0][2]; a[0][2] = a[2][0]; a[2][0] = temp;
		temp = a[0][3]; a[0][3] = a[3][0]; a[3][0] = temp;
		temp = a[1][2]; a[1][2] = a[2][1]; a[2][1] = temp;
		temp = a[1][3]; a[1][3] = a[3][1]; a[3][1] = temp;
		temp = a[2][3]; a[2][3] = a[3][2]; a[3][2] = temp;
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
	Matrix operator* (const Matrix& matrix)
	{
		return mul(matrix);
	}

	// invert the matrix
	Matrix invert()
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
		if (det == 0) {
			// Handle this case
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}
};


