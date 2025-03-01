#include "PixelShader.h"
#include "BindableCodex.h"

namespace Bind
{
	PixelShader::PixelShader(D3DApp& d3dApp, const std::string& path)
		:
		path(path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBlob);
		GetDevice(d3dApp)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	}

	void PixelShader::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
	std::shared_ptr<PixelShader> PixelShader::Resolve(D3DApp& d3dApp, const std::string& path)
	{
		return Codex::Resolve<PixelShader>(d3dApp, path);
	}
	std::string PixelShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}
	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}