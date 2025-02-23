#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <algorithm>

using namespace DirectX;

class MathHelper
{
public:
	XMFLOAT3 ComputeNormal(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2);
	void TransposeNormal(XMFLOAT3& normal, const XMFLOAT4X4& worldMatrix);

	template <typename T>
	static constexpr auto sq(const T& x)
	{
		return x * x;
	}

	template<typename T>
	static constexpr T wrap_angle(T theta)
	{
		const T modded = fmod(theta, (T)2.0 * (T)PiD);
		return (modded > (T)PiD) ?
			(modded - (T)2.0 * (T)PiD) :
			modded;
	}

	static const float Infinity;
	static const float Pi;
	static const double PiD;
};
