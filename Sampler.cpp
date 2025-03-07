#include "Sampler.h"
#include "BindableCodex.h"

namespace Bind
{
	Sampler::Sampler(D3DApp& d3dApp)
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		GetDevice(d3dApp)->CreateSamplerState(&samplerDesc, &pSampler);
	}

	void Sampler::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
	std::shared_ptr<Sampler> Sampler::Resolve(D3DApp& d3dApp)
	{
		return Codex::Resolve<Sampler>(d3dApp);
	}
	std::string Sampler::GenerateUID()
	{
		return typeid(Sampler).name();
	}
	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID();
	}
}