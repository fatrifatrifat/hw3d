#include "Bindable.h"

ID3D11Device* Bindable::GetDevice(D3DApp& d3dApp) noexcept
{
	return d3dApp.GetDevice();
}

ID3D11DeviceContext* Bindable::GetContext(D3DApp& d3dApp) noexcept
{
	return d3dApp.GetDeviceContext();
}
