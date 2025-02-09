//***************************************************************************************
// MathHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "MathHelper.h"
#include <float.h>
#include <cmath>

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;

XMFLOAT3 MathHelper::ComputeNormal(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2)
{
	XMVECTOR v0 = XMLoadFloat3(&p0);
	XMVECTOR v1 = XMLoadFloat3(&p1);
	XMVECTOR v2 = XMLoadFloat3(&p2);

	XMVECTOR u = XMVectorSubtract(v1, v0);
	XMVECTOR v = XMVectorSubtract(v2, v0);

	XMVECTOR normal = XMVector3Cross(u, v);

	normal = XMVector3Normalize(normal);

	XMFLOAT3 result;
	XMStoreFloat3(&result, normal);

	return result;
}

void MathHelper::TransposeNormal(const XMFLOAT3& normal, const XMFLOAT4X4& worldMatrix)
{
	XMVECTOR n = XMLoadFloat3(&normal);
	XMMATRIX W = XMLoadFloat4x4(&worldMatrix);

	XMMATRIX W_invTranspose = XMMatrixTranspose(XMMatrixInverse(nullptr, W));

	n = XMVector3TransformNormal(n, W_invTranspose);
	n = XMVector3Normalize(n);

	XMStoreFloat3(&normal, n);
}
