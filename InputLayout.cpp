#include "InputLayout.h"

InputLayout::InputLayout(D3DApp& d3dApp, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	GetDevice(d3dApp)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&pInputLayout
	);
}

void InputLayout::Bind(D3DApp& d3dApp) noexcept
{
	GetContext(d3dApp)->IASetInputLayout(pInputLayout.Get());
}
