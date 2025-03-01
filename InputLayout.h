#pragma once
#include "Bindable.h"
#include "Vertex.h"

namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(D3DApp& d3dApp,
			Dvtx::VertexLayout layout,
			ID3DBlob* pVertexShaderBytecode);
		void Bind(D3DApp& d3dApp) noexcept override;
		static std::shared_ptr<InputLayout> Resolve(D3DApp& d3dApp,
			const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode);
		static std::string GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
		std::string GetUID() const noexcept override;
	protected:
		Dvtx::VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}