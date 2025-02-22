#pragma once
#include "Bindable.h"

namespace Bind
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(D3DApp& d3dApp, const std::wstring& filename);
		void Bind(D3DApp& d3dApp) noexcept override;
		ID3DBlob* GetBytecode() const noexcept;

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}