#include "../WinMain.h"
#include "Vector3.h"
#include <math.h>

// �R���X�g���N�^
Vector3::Vector3() :
	x(-1.0f), y(-1.0f), z(-1.0f)
{
}

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3::~Vector3()
{
}

// ������
void Vector3::Clear()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

// �l���P���Z�b�g
void Vector3::Set(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

// �l��Vector3�ňꊇ�Z�b�g
void Vector3::Set(const Vector3& _v)
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

// �������擾
float Vector3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

// ���K��
void Vector3::Normalize()
{
	float length = GetLength();
	if (length > 0.0f)
	{
		x = x / length;
		y = y / length;
		z = z / length;
	}
}

// ������ݒ�
void Vector3::SetLength(float _length)
{
	Normalize();
	x = x * _length;
	y = y * _length;
	z = z * _length;
}

// = ���Z�q�I�[�o�[���[�h
Vector3& Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

// += ���Z�q�I�[�o�[���[�h
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

// -= ���Z�q�I�[�o�[���[�h
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

// *= ���Z�q�I�[�o�[���[�h
Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3& Vector3::operator*=(const float& f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

// /= ���Z�q�I�[�o�[���[�h
Vector3& Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

VECTOR Vector3::GetVECTOR() const
{
	VECTOR v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

void Vector3::SetVECTOR(const VECTOR& _v)
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

// + ���Z�q�I�[�o�[���[�h
Vector3 operator+(const Vector3& _v0, const Vector3& _v1)
{
	Vector3 v;
	v.x = _v0.x + _v1.x;
	v.y = _v0.y + _v1.y;
	v.z = _v0.z + _v1.z;
	return v;
}

// - ���Z�q�I�[�o�[���[�h
Vector3 operator-(const Vector3& _v0, const Vector3& _v1)
{
	Vector3 v;
	v.x = _v0.x - _v1.x;
	v.y = _v0.y - _v1.y;
	v.z = _v0.z - _v1.z;
	return v;
}

// * ���Z�q�I�[�o�[���[�h
Vector3 operator*(const Vector3& _v0, const float _f)
{
	Vector3 v;
	v.x = _v0.x * _f;
	v.y = _v0.y * _f;
	v.z = _v0.z * _f;
	return v;
}

// / ���Z�q�I�[�o�[���[�h
Vector3 operator/(const Vector3& _v0, const float _f)
{
	Vector3 v;
	v.x = _v0.x / _f;
	v.y = _v0.y / _f;
	v.z = _v0.z / _f;
	return v;
}

// �Q��Vector3�̋������v�Z����
float GetVector3Distance(const Vector3& _v0, const Vector3& _v1)
{
	Vector3 v = _v0 - _v1;
	return v.GetLength();
}

// �Q��Vector3�̓��ς��v�Z����
float GetVector3Dot(const Vector3& _v0, const Vector3& _v1)
{
	return _v0.x * _v1.x + _v0.y * _v1.y + _v0.z * _v1.z;
}

// �Q��Vector3�̊O�ς��v�Z����
Vector3 GetVector3Cross(const Vector3& _v0, const Vector3& _v1)
{
	Vector3 v;
	v.x = _v0.y * _v1.z - _v0.z * _v1.y;
	v.y = _v0.z * _v1.x - _v0.x * _v1.z;
	v.z = _v0.x * _v1.y - _v0.y * _v1.x;
	return v;
}


