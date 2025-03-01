#pragma once
#include "Bindable.h"

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(D3DApp& d3dApp, const std::string& path, UINT slot = 0);
		void Bind(D3DApp& d3dApp) noexcept override;
		static std::shared_ptr<Texture> Resolve(D3DApp& d3dApp, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		std::string GetUID() const noexcept override;
	private:
		unsigned int slot;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}