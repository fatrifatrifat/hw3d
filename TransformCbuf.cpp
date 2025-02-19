#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(D3DApp& d3dApp, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(d3dApp);
	}
}

void TransformCbuf::Bind(D3DApp& d3dApp) noexcept
{
	const auto model = parent.GetTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			d3dApp.GetCamera() *
			d3dApp.GetProjMatrix()
		)
	};
	pVcbuf->Update(d3dApp, tf);
	pVcbuf->Bind(d3dApp);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;