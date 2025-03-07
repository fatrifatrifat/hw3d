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
	static T wrap_angle(T theta)
	{
		const T twoPi = (T)2 * (T)MathHelper::PiD;
		const T mod = fmod(theta, twoPi);
		if (mod > (T)MathHelper::PiD)
		{
			return mod - twoPi;
		}
		else if (mod < (T)MathHelper::PiD)
		{
			return mod + twoPi;
		}
		return mod;
	}


	static const float Infinity;
	static const float Pi;
	static const double PiD;
};
