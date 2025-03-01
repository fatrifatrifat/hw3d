#pragma once
#include "D3DApp.h"

namespace Bind
{
	class Bindable
	{
	public:
		virtual void Bind(D3DApp& d3dApp) noexcept = 0;
		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}
		virtual ~Bindable() = default;
	protected:
		static ID3D11DeviceContext* GetContext(D3DApp& d3dApp) noexcept;
		static ID3D11Device* GetDevice(D3DApp& d3dApp) noexcept;
	};
}