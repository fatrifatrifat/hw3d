#pragma once
#include "Bindable.h"

namespace Bind
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		void Update(D3DApp& d3dApp, const C& consts)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			GetContext(d3dApp)->Map(
				pConstantBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&msr
			);
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(d3dApp)->Unmap(pConstantBuffer.Get(), 0u);
		}
		ConstantBuffer(D3DApp& d3dApp, const C& consts, UINT slot = 0u)
			: slot(slot)
		{
			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(consts);
			cbd.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = &consts;
			GetDevice(d3dApp)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
		}
		ConstantBuffer(D3DApp& d3dApp, UINT slot = 0u)
			: slot(slot)
		{

			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(C);
			cbd.StructureByteStride = 0u;
			GetDevice(d3dApp)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
		UINT slot;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using ConstantBuffer<C>::slot;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(D3DApp& d3dApp) noexcept override
		{
			GetContext(d3dApp)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using ConstantBuffer<C>::slot;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(D3DApp& d3dApp) noexcept override
		{
			GetContext(d3dApp)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};
}