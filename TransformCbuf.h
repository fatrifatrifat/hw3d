#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(D3DApp& d3dApp, const Drawable& parent);
	void Bind(D3DApp& d3dApp) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf;
	const Drawable& parent;
};