#pragma once
#include "../external/hlslpp/include/hlsl++.h"
#include "WinMain.h"
#include "Class/Vector3.h"
using namespace hlslpp;

inline
float4 cast(const VECTOR& v)
{
	return float4(v.x, v.y, v.z, 1.0f);
}

inline
VECTOR cast(const float3& v)
{
	return VGet(v.x, v.y, v.z);
}

inline
VECTOR cast(const float4& v)
{
	return VGet(v.x, v.y, v.z);
}

inline
float4x4 cast(const MATRIX& m)
{
	float4x4	result;
	result._11_12_13_14 = float4(m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	result._21_22_23_24 = float4(m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	result._31_32_33_34 = float4(m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	result._41_42_43_44 = float4(m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
	return result;
}

inline
float4 cast(const Vector3& v)
{
	return float4(v.x, v.y, v.z, 1.0f);
}