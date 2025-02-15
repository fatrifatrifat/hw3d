#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(D3DApp& d3dApp, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(d3dApp);
	}
}

void TransformCbuf::Bind(D3DApp& d3dApp) noexcept
{
	pVcbuf->Update(d3dApp,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * d3dApp.GetCamera() * d3dApp.GetProjMatrix()
		)
	);
	pVcbuf->Bind(d3dApp);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pVcbuf;