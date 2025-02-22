#pragma once
#include "DrawableCommon.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(D3DApp& d3dApp, float radius);
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};