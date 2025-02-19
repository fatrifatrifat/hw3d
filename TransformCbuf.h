#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};

public:
	TransformCbuf(D3DApp& d3dApp, const Drawable& parent, UINT slot = 0u);
	void Bind(D3DApp& d3dApp) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};