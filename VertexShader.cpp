#include "VertexShader.h"
#include "BindableCodex.h"
#include <typeinfo>

namespace Bind
{
	VertexShader::VertexShader(D3DApp& d3dApp, const std::string& path)
		:
		path(path)
	{
		D3DReadFileToBlob(std::wstring{ path.begin(),path.end() }.c_str(), &pBytecodeBlob);
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
	std::shared_ptr<VertexShader> VertexShader::Resolve(D3DApp& d3dApp, const std::string& path)
	{
		return Codex::Resolve<VertexShader>(d3dApp, path);
	}
	std::string VertexShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}