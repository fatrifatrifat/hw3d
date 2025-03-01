#pragma once
#include "Bindable.h"

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(D3DApp& d3dApp, const std::vector<unsigned int>& indices);
		IndexBuffer(D3DApp& d3dApp, std::string tag, const std::vector<unsigned int>& indices);
		void Bind(D3DApp& d3dApp) noexcept override;
		UINT GetCount() const noexcept;
		static std::shared_ptr<IndexBuffer> Resolve(D3DApp& d3dApp, const std::string& tag,
			const std::vector<unsigned int>& indices);
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		std::string GetUID() const noexcept override;
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string tag;
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}