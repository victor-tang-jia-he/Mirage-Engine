#include "../Include/Vector3.h"
#include "../Include/Matrix3.h"
using namespace Bwt::Core::Maths;
Vector3 Vector3::GetOpposed() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::AddVector3(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::AddScalar(float scalar) const
{
	return Vector3(x + scalar, y + scalar, z + scalar);
}

Vector3 Vector3::ElementWiseProduct(const Vector3& other) const
{
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::ProductScalar(float scalar) const
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::GetMid(const Vector3& other) const
{
	return AddVector3(other).ProductScalar(1.f / 2.f);
}

float Vector3::GetDistance(const Vector3& other) const
{
	return sqrt(GetDistancePow(other));
}

float Vector3::GetDistancePow(const Vector3& other) const
{
	float dx = other.x - x;
	float dy = other.y - y;
	float dz = other.z - z;
	return dx * dx + dy * dy + dz * dz;
}

float Vector3::GetNorm() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::GetSquareNorm() const
{
	return x * x + y * y + z * z;
}

Vector3 Vector3::Normalized() const
{
	Vector3 vec;
	float magnitude = GetNorm();

	if (magnitude == 0)
	{
		return Vector3(0, 0, 0);
	}

	vec.x = x / magnitude;
	vec.y = y / magnitude;
	vec.z = z / magnitude;
	return vec;
}

float Vector3::GetPerspective(float t) const
{
	if (t <= 0) return 0;

	return GetSquareNorm() / t;
}

Vector3 Bwt::Core::Maths::Vector3::Reflect(Vector3 I, Vector3 N)
{
	// R = I - 2 * (I ¡¤ N) * N
	return I - N.Normalized() * (2.0f * I.DotProductVector3(N.Normalized()));
}

float Vector3::DotProductVector3(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::CrossProductVector3(const Vector3& other) const
{
	return Vector3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	);
}

float Vector3::GetAngle(const Vector3& v2) const
{
	float denom = GetNorm() * v2.GetNorm();
	if (denom == 0.0f) return 0.0f;

	return acosf(DotProductVector3(v2) / denom) * (180.f / PI);
}

void Vector3::Rotation(float angleX, float angleY, float angleZ) {
	float cosX = cosf(angleX), sinX = sinf(angleX);
	float cosY = cosf(angleY), sinY = sinf(angleY);
	float cosZ = cosf(angleZ), sinZ = sinf(angleZ);

	float x0 = x, y0 = y, z0 = z;

	float y1 = y0 * cosX - z0 * sinX;
	float z1 = y0 * sinX + z0 * cosX;

	float x2 = x0 * cosY + z1 * sinY;
	float z2 = -x0 * sinY + z1 * cosY;

	x = x2 * cosZ - y1 * sinZ;
	y = x2 * sinZ + y1 * cosZ;
	z = z2;
}

std::array<float, 3> Vector3::AsList() const
{
	return { x, y, z };
}

std::ostream& Bwt::Core::Maths::operator<<(std::ostream& os, const Vector3& vec)
{
	os << vec.x << " ; " << vec.y << " ; " << vec.z;
	return os;
}
