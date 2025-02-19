#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(D3DApp& d3dApp, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(d3dApp, slot);
	}
}

void TransformCbuf::Bind(D3DApp& d3dApp) noexcept
{
	const auto modelView = parent.GetTransformXM() * d3dApp.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			d3dApp.GetProjMatrix()
		)
	};
	pVcbuf->Update(d3dApp, tf);
	pVcbuf->Bind(d3dApp);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;