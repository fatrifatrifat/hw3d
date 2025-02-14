#include "VertexShader.h"

VertexShader::VertexShader(D3DApp& d3dApp, const std::wstring& filename)
{
	D3DReadFileToBlob(filename.c_str(), &pBytecodeBlob);
	GetDevice(d3dApp)->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
}

void VertexShader::Bind(D3DApp& d3dApp) noexcept
{
	GetContext(d3dApp)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}