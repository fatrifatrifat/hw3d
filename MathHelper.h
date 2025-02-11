#pragma once
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

class MathHelper
{
public:
	XMFLOAT3 ComputeNormal(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2);
	void TransposeNormal(XMFLOAT3& normal, const XMFLOAT4X4& worldMatrix);


	static const float Infinity;
	static const float Pi;
};
