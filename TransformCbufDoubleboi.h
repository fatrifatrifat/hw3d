#pragma once
#include "TransformCbuf.h"

namespace Bind
{
	class TransformCbufDoubleboi : public TransformCbuf
	{
	public:
		TransformCbufDoubleboi(D3DApp& d3dApp, const Drawable& parent, UINT slotV = 0u, UINT slotP = 0u);
		void Bind(D3DApp& d3dApp) noexcept override;
	protected:
		void UpdateBindImpl(D3DApp& d3dApp, const Transforms& tf) noexcept;
	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
	};
}