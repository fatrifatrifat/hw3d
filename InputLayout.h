#pragma once
#include "Bindable.h"

namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(D3DApp& d3dApp,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* pVertexShaderByteCode);
		void Bind(D3DApp& d3dApp) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}