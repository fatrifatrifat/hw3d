#include "PixelShader.h"

namespace Bind
{
	PixelShader::PixelShader(D3DApp& d3dApp, const std::wstring& filename)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(filename.c_str(), &pBlob);
		GetDevice(d3dApp)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	}

	void PixelShader::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
}