#pragma once
#include "TestObject.h"

class Skull : public TestObject<Skull>
{
public:
	Skull(D3DApp& d3dApp, std::mt19937& rng,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		DirectX::XMFLOAT3 material);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 material;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} colorConst;

	// model transform
	DirectX::XMFLOAT3X3 mt;
};

