#pragma once
#include "Bindable.h"

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(D3DApp& d3dApp, const std::vector<UINT>& indices);
		void Bind(D3DApp& d3dApp) noexcept override;
		UINT GetCount() const noexcept;

	protected:
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}