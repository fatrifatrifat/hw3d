#include "Bindable.h"

namespace Bind
{
	ID3D11DeviceContext* Bindable::GetContext(D3DApp& d3dApp) noexcept
	{
		return d3dApp.GetDeviceContext();
	}

	ID3D11Device* Bindable::GetDevice(D3DApp& d3dApp) noexcept
	{
		return d3dApp.GetDevice();
	}
}