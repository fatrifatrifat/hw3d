#include "TransformCbuf.h"

namespace Bind
{
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
		UpdateBindImpl(d3dApp, GetTransforms(d3dApp));
	}

	void TransformCbuf::UpdateBindImpl(D3DApp& d3dApp, const Transforms& tf) noexcept
	{
		pVcbuf->Update(d3dApp, tf);
		pVcbuf->Bind(d3dApp);
	}

	TransformCbuf::Transforms TransformCbuf::GetTransforms(D3DApp& d3dApp) noexcept
	{
		const auto modelView = parent.GetTransformXM() * d3dApp.GetCamera();
		return {
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView *
				d3dApp.GetProjMatrix()
			)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
}