#include "VertexBuffer.h"

void VertexBuffer::Bind(D3DApp& d3dApp) noexcept
{
	const UINT offset = 0u;
	GetContext(d3dApp)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
