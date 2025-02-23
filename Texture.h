#pragma once
#include "Bindable.h"

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(D3DApp& d3dApp, const class Surface& s, unsigned int slot = 0);
		void Bind(D3DApp& d3dApp) noexcept override;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}