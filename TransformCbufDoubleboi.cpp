#include "TransformCbufDoubleboi.h"

namespace Bind
{
	TransformCbufDoubleboi::TransformCbufDoubleboi(D3DApp& d3dApp, const Drawable& parent, UINT slotV, UINT slotP)
		:
		TransformCbuf(d3dApp, parent, slotV)
	{
		if (!pPcbuf)
		{
			pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(d3dApp, slotP);
		}
	}

	void Bind::TransformCbufDoubleboi::Bind(D3DApp& d3dApp) noexcept
	{
		const auto tf = GetTransforms(d3dApp);
		TransformCbuf::UpdateBindImpl(d3dApp, tf);
		UpdateBindImpl(d3dApp, tf);
	}

	void TransformCbufDoubleboi::UpdateBindImpl(D3DApp& d3dApp, const Transforms& tf) noexcept
	{
		pPcbuf->Update(d3dApp, tf);
		pPcbuf->Bind(d3dApp);
	}

	std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> TransformCbufDoubleboi::pPcbuf;
}