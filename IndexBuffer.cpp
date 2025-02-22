#include "IndexBuffer.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(D3DApp& d3dApp, const std::vector<UINT>& indices)
		: count((UINT)indices.size())
	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = UINT(sizeof(UINT) * count);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = sizeof(UINT);

		D3D11_SUBRESOURCE_DATA iinitData = {};
		iinitData.pSysMem = indices.data();

		HRESULT hr = GetDevice(d3dApp)->CreateBuffer(&ibd, &iinitData, &pIndexBuffer);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to create index buffer", 0, 0);
		}
	}

	void IndexBuffer::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}