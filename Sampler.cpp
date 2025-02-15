#include "Sampler.h"

Sampler::Sampler(D3DApp& d3dApp)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GetDevice(d3dApp)->CreateSamplerState(&samplerDesc, &pSampler);
}

void Sampler::Bind(D3DApp& d3dApp) noexcept
{
	GetContext(d3dApp)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}