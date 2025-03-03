#include "Texture.h"
#include "Surface.h"
#include "BindableCodex.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(D3DApp& d3dApp, const std::string& path, UINT slot)
		:
		path(path),
		slot(slot)
	{
		// load surface
		const auto s = Surface::FromFile(path);
		hasAlpha = s.AlphaLoaded();

		// create texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.GetBufferPtr();
		sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GetDevice(d3dApp)->CreateTexture2D(
			&textureDesc, &sd, &pTexture
		);

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GetDevice(d3dApp)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		);
	}

	void Texture::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}
	std::shared_ptr<Texture> Texture::Resolve(D3DApp& d3dApp, const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture>(d3dApp, path, slot);
	}
	std::string Texture::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
	}
	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID(path, slot);
	}
	bool Texture::HasAlpha() const noexcept
	{
		return hasAlpha;
	}
}