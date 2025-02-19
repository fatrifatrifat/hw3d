#pragma once
#include "D3DApp.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(D3DApp& d3dApp, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(D3DApp& d3dApp) const noexcept;
	void Bind(D3DApp& d3dApp) const noexcept;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};