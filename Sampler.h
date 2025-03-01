#pragma once
#include "Bindable.h"

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(D3DApp& d3dApp);
		void Bind(D3DApp& d3dApp) noexcept override;
		static std::shared_ptr<Sampler> Resolve(D3DApp& d3dApp);
		static std::string GenerateUID();
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}