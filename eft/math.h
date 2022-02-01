struct Vector2
{
	float x, y;

	inline Vector2() {
		x = y = 0.0f;
	}

	inline Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	inline Vector2 operator - () { return Vector2(-x, -y); }
	inline Vector2 operator + (Vector2 v) const { return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator - (Vector2 v) const { return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator * (Vector2 v) const { return Vector2(x * v.x, y * v.y); }
	inline Vector2 operator / (Vector2 v) const { return Vector2(x / v.x, y / v.y); }

	inline Vector2 operator = (Vector2 v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}

	inline Vector2 operator += (Vector2 v)
	{
		this->x += v.x;
		this->y += v.y;

		return *this;
	}

	inline Vector2 operator -= (Vector2 v)
	{
		this->x -= v.x;
		this->y -= v.y;

		return *this;
	}

	inline Vector2 operator /= (Vector2 v)
	{
		this->x /= v.x;
		this->y /= v.y;

		return *this;
	}

	inline Vector2 operator *= (Vector2 v)
	{
		this->x *= v.x;
		this->y *= v.y;

		return *this;
	}

	inline Vector2 operator + (float f) const { return Vector2(x + f, y + f); }
	inline Vector2 operator - (float f) const { return Vector2(x - f, y - f); }
	inline Vector2 operator * (float f) const { return Vector2(x * f, y * f); }
	inline Vector2 operator / (float f) const { return Vector2(x / f, y / f); }

	inline Vector2 operator = (float f)
	{
		this->x = f;
		this->y = f;

		return *this;
	}

	inline Vector2 operator += (float f)
	{
		this->x += f;
		this->y += f;

		return *this;
	}

	inline Vector2 operator -= (float f)
	{
		this->x -= f;
		this->y -= f;

		return *this;
	}

	inline Vector2 operator /= (float f)
	{
		this->x /= f;
		this->y /= f;

		return *this;
	}

	inline Vector2 operator *= (float f)
	{
		this->x *= f;
		this->y *= f;

		return *this;
	}

	inline bool operator == (Vector2 v) const { return ((x == v.x) && (y == v.y)); }
	inline bool operator != (Vector2 v) const { return ((x != v.x) || (y != v.y)); }

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}
};
struct Vector3
{
	float x, y, z;

	inline Vector3() {
		x = y = z = 0.0f;
	}

	inline Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline Vector3 operator - () { return Vector3(-x, -y, -z); }

	inline Vector3 operator + (Vector3 v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator - (Vector3 v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator * (Vector3 v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
	inline Vector3 operator / (Vector3 v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

	inline Vector3 operator = (Vector3 v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	inline Vector3 operator += (Vector3 v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}

	inline Vector3 operator -= (Vector3 v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	inline Vector3 operator /= (Vector3 v)
	{
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

		return *this;
	}

	inline Vector3 operator *= (Vector3 v)
	{
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

	inline Vector3 operator + (float f) const { return Vector3(x + f, y + f, z + f); }
	inline Vector3 operator - (float f) const { return Vector3(x - f, y - f, z - f); }
	inline Vector3 operator * (float f) const { return Vector3(x * f, y * f, z * f); }
	inline Vector3 operator / (float f) const { return Vector3(x / f, y / f, z / f); }


	inline Vector3 operator = (float f)
	{
		this->x = f;
		this->y = f;
		this->z = f;

		return *this;
	}

	inline Vector3 operator += (float f)
	{
		this->x += f;
		this->y += f;
		this->z += f;

		return *this;
	}

	inline Vector3 operator -= (float f)
	{
		this->x -= f;
		this->y -= f;
		this->z -= f;

		return *this;
	}

	inline Vector3 operator /= (float f)
	{
		this->x /= f;
		this->y /= f;
		this->z /= f;

		return *this;
	}

	inline Vector3 operator *= (float f)
	{
		this->x *= f;
		this->y *= f;
		this->z *= f;

		return *this;
	}

	inline bool operator == (Vector3 v) const { return ((x == v.x) && (y == v.y) && (z == v.z)); }
	inline bool operator != (Vector3 v) const { return ((x != v.x) || (y != v.y) || (z != v.z)); }
	inline bool operator == (float f) const { return ((x == f) && (y == f) && (z == f)); }

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline bool Zero() const {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
	}
};
struct Vector4
{
	float x, y, z, w;

	inline Vector4() {
		x = y = z = w = 0.0f;
	}

	inline Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline Vector4 operator - () { return Vector4(-x, -y, -z, -w); }

	inline Vector4 operator + (Vector4 v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Vector4 operator - (Vector4 v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline Vector4 operator * (Vector4 v) const { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
	inline Vector4 operator / (Vector4 v) const { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); }

	inline Vector4 operator = (const Vector4 v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;

		return *this;
	}

	inline Vector4 operator += (const Vector4 v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		this->w += v.w;

		return *this;
	}

	inline Vector4 operator -= (const Vector4 v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		this->w -= v.w;

		return *this;
	}

	inline Vector4 operator /= (const Vector4 v)
	{
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		this->w /= v.w;

		return *this;
	}

	inline Vector4 operator *= (const Vector4 v)
	{
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		this->w *= v.w;

		return *this;
	}

	inline Vector4 operator + (float f) const { return Vector4(x + f, y + f, z + f, w + f); }
	inline Vector4 operator - (float f) const { return Vector4(x - f, y - f, z - f, w - f); }
	inline Vector4 operator * (float f) const { return Vector4(x * f, y * f, z * f, w * f); }
	inline Vector4 operator / (float f) const { return Vector4(x / f, y / f, z / f, w / f); }

	inline Vector4 operator = (float f)
	{
		this->x = f;
		this->y = f;
		this->z = f;
		this->w = f;

		return *this;
	}

	inline Vector4 operator += (float f)
	{
		this->x += f;
		this->y += f;
		this->z += f;
		this->w += f;

		return *this;
	}

	inline Vector4 operator -= (float f)
	{
		this->x -= f;
		this->y -= f;
		this->z -= f;
		this->w -= f;

		return *this;
	}

	inline Vector4 operator /= (float f)
	{
		this->x /= f;
		this->y /= f;
		this->z /= f;
		this->w /= f;

		return *this;
	}

	inline Vector4 operator *= (float f)
	{
		this->x *= f;
		this->y *= f;
		this->z *= f;
		this->w *= f;

		return *this;
	}

	bool operator == (Vector4 v) const { return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)); }
	bool operator != (Vector4 v) const { return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w)); }

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}
};
struct Matrix {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		}; float m[4][4];
	};
};

namespace Math
{
#define CONST_PI                                                 3.1415926535897932f
#define CONST_RadToDeg                                           57.295779513082321600f
#define CONST_DegToRad                                           0.017453292519943296f
#define CONST_UnrRotToRad                                        0.00009587379924285f
#define CONST_RadToUnrRot                                        10430.3783504704527f
#define powf(a, b) ((a) * (a))

	__forceinline float sinf(float a)
	{
		return __sinf(a);
	}
	__forceinline float cosf(float a)
	{
		return __cosf(a);
	}
	__forceinline float asinf(float a)
	{
		return __asinf(a);
	}
	__forceinline float acosf(float a)
	{
		return __acosf(a);
	}
	__forceinline float atan2f(float x, float y)
	{
		return __atan2f(x, y);
	}
	__forceinline float atanf(float a) {
		return __atanf(a);
	}
	__forceinline float tanf(float a) {
		return __tanf(a);
	}
	__forceinline float frac(float v)
	{
		return v - floor(v);
	}
	__forceinline float fmodf(float a, float b)
	{
		float c = frac(abs(a / b)) * abs(b);
		return (a < 0) ? -c : c;
	}
	__forceinline float sqrt(float x) {
		union { int i; float x; } u;
		u.x = x; u.i = (u.i >> 1) + 0x1FC00000;
		u.x = u.x + x / u.x;
		return .25f * u.x + x / u.x;
	}

	__forceinline float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	__forceinline float Dist3D(const Vector3& Src, const Vector3& Dst) {
		return sqrt(powf(Src.x - Dst.x, Src.x - Dst.x) + powf(Src.y - Dst.y, Src.x - Dst.x) + powf(Src.z - Dst.z, Src.z - Dst.z));
	}

	__forceinline float Dist2D(const Vector2& Src, const Vector2& Dst)
	{
		return sqrt(powf(Src.x - Dst.x, Src.x - Dst.x) + powf(Src.y - Dst.y, Src.x - Dst.x));
	}

	__forceinline float Vec3Length(Vector3& v)
	{
		return sqrtf(powf(v.x, 2.f) + powf(v.y, 2.f) + powf(v.z, 2.f));
	}

	__forceinline void Vector3Normalize(Vector3& v)
	{
		float length = Vec3Length(v);
		if (!length) v.x = v.y = v.z = 1.f;
		else v /= length;
	}

	__forceinline Vector2 CalcAngle(const Vector3& Src, const Vector3& Dst) {
		Vector2 Angles; Vector3 Delta = Src - Dst;
		Angles.x = (-atan2f(Delta.x, -Delta.z) * CONST_RadToDeg);
		Angles.y = (asinf(Delta.y / Vec3Length(Delta)) * CONST_RadToDeg);
		return Angles;
	}

	__forceinline void Normalize(Vector2& Angle)
	{
		if (Angle.y < -89.f) Angle.y = -89.f;
		else if (Angle.y > 89.f) Angle.y = 89.f;
		if (Angle.x < -360.f) Angle.x += 360.f;
		else if (Angle.x > 360.f)  Angle.x -= 360.f;
	}

	__forceinline void Clamp(Vector2& Angle)
	{
		if (Angle.y < -89.f) Angle.y = -89.f;
		if (Angle.y > 89.f) Angle.y = 89.f;
		while (Angle.x < -180.f) Angle.x += 360.f;
		while (Angle.x > 180.f) Angle.x -= 360.f;
	}

	__forceinline void SmoothAngle(Vector2 Src, Vector2& Dst, float FOV) {
		Vector2 Delta = Dst - Src;
		Math::Clamp(Delta);
		Dst = Src + Delta / FOV;
	}
}