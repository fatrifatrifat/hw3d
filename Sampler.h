#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(D3DApp& d3dApp);
	void Bind(D3DApp& d3dApp) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};