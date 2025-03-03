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
	void Draw(D3DApp& d3dApp) const;
	void Bind(D3DApp& d3dApp, DirectX::FXMMATRIX view) const noexcept; 
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
	static constexpr float travelSpeed = 12.0f;
};