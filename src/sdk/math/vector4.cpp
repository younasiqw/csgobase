#include "vector4.h"

#include <cmath>
#include <limits>

void VectorCopy(const Vector4& src, Vector4& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	dst.w = src.w;
}
void VectorLerp(const Vector4& src1, const Vector4& src2, vec_t t, Vector4& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
	dest.w = src1.w + (src2.w - src1.w) * t;
}
float VectorLength(const Vector4& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

vec_t NormalizeVector(Vector4& v)
{
	vec_t l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	} else
	{
		v.x = v.y = v.z = v.w = 0.0f;
	}
	return l;
}

Vector4::Vector4(void)
{
	Invalidate();
}
Vector4::Vector4(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}
Vector4::Vector4(vec_t* clr)
{
	x = clr[0];
	y = clr[1];
	z = clr[2];
	w = clr[3];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void Vector4::Init(vec_t ix, vec_t iy, vec_t iz, vec_t iw)
{
	x = ix; y = iy; z = iz; w = iw;
}

void Vector4::Random(vec_t minVal, vec_t maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	w = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
void Vector4::Zero()
{
	x = y = z = w = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

Vector4& Vector4::operator=(const Vector4 &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
	return *this;
}


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
vec_t& Vector4::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t Vector4::operator[](int i) const
{
	return ((vec_t*)this)[i];
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
vec_t* Vector4::Base()
{
	return (vec_t*)this;
}

vec_t const* Vector4::Base() const
{
	return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool Vector4::IsValid() const
{
	return !isinf(x) && !isinf(y) && !isinf(z) && !isinf(w);
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

void Vector4::Invalidate()
{
	//#ifdef _DEBUG
	//#ifdef VECTOR_PARANOIA
	x = y = z = w = std::numeric_limits<float>::infinity();
	//#endif
	//#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool Vector4::operator==(const Vector4& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

bool Vector4::operator!=(const Vector4& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
void Vector4::CopyToArray(float* rgfl) const
{
	rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; rgfl[3] = w;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------
// #pragma message("TODO: these should be SSE")

void Vector4::Negate()
{
	x = -x; y = -y; z = -z; w = -w;
}

// Get the component of this vector parallel to some other given vector
Vector4 Vector4::ProjectOnto(const Vector4& onto)
{
	return onto * (this->Dot(onto) / (onto.LengthSqr()));
}

// FIXME: Remove
// For backwards compatability
void Vector4::MulAdd(const Vector4& a, const Vector4& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
	w = a.w + b.w * scalar;
}

Vector4 VectorLerp(const Vector4& src1, const Vector4& src2, vec_t t)
{
	Vector4 result;
	VectorLerp(src1, src2, t, result);
	return result;
}

vec_t Vector4::Dot(const Vector4& b) const
{
	return (x*b.x + y*b.y + z*b.z + w*b.w);
}
void VectorClear(Vector4& a)
{
	a.x = a.y = a.z = a.w = 0.0f;
}

vec_t Vector4::Length(void) const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

// check a point against a box
bool Vector4::WithinAABox(Vector4 const &boxmin, Vector4 const &boxmax)
{
	return (
		(x >= boxmin.x) && (x <= boxmax.x) &&
		(y >= boxmin.y) && (y <= boxmax.y) &&
		(z >= boxmin.z) && (z <= boxmax.z) &&
		(w >= boxmin.w) && (w <= boxmax.w)
		);
}

//-----------------------------------------------------------------------------
// Get the distance from this vector to the other one 
//-----------------------------------------------------------------------------
vec_t Vector4::DistTo(const Vector4 &vOther) const
{
	Vector4 delta;
	delta = *this - vOther;
	return delta.Length();
}

//-----------------------------------------------------------------------------
// Returns a vector with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------
Vector4 Vector4::Min(const Vector4 &vOther) const
{
	return Vector4(x < vOther.x ? x : vOther.x,
					y < vOther.y ? y : vOther.y,
					z < vOther.z ? z : vOther.z,
					w < vOther.w ? w : vOther.w);
}

Vector4 Vector4::Max(const Vector4 &vOther) const
{
	return Vector4(x > vOther.x ? x : vOther.x,
					y > vOther.y ? y : vOther.y,
					z > vOther.z ? z : vOther.z,
					w > vOther.w ? w : vOther.w);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

Vector4 Vector4::operator-(void) const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+(const Vector4& v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator-(const Vector4& v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator*(float fl) const
{
	return Vector4(x * fl, y * fl, z * fl, w * fl);
}

Vector4 Vector4::operator*(const Vector4& v) const
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator/(float fl) const
{
	return Vector4(x / fl, y / fl, z / fl, w / fl);
}

Vector4 Vector4::operator/(const Vector4& v) const
{
	return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4 operator*(float fl, const Vector4& v)
{
	return v * fl;
}