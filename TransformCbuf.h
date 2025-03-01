#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

namespace Bind
{
	class TransformCbuf : public Bindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX modelView;
			DirectX::XMMATRIX modelViewProj;
		};
	public:
		TransformCbuf(D3DApp& d3dApp, const Drawable& parent, UINT slot = 0u);
		void Bind(D3DApp& d3dApp) noexcept override;
	protected:
		void UpdateBindImpl(D3DApp& d3dApp, const Transforms& tf) noexcept;
		Transforms GetTransforms(D3DApp& d3dApp) noexcept;
	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Drawable& parent;
	};
}