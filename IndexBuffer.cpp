#include "IndexBuffer.h"
#include "BindableCodex.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(D3DApp& d3dApp, const std::vector<unsigned int>& indices)
		:
		IndexBuffer(d3dApp, "?", indices)
	{}
	IndexBuffer::IndexBuffer(D3DApp& d3dApp, std::string tag, const std::vector<unsigned int>& indices)
		:
		tag(tag),
		count((UINT)indices.size())
	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(count * sizeof(unsigned int));
		ibd.StructureByteStride = sizeof(unsigned int);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		GetDevice(d3dApp)->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	}

	void IndexBuffer::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
	std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(D3DApp& d3dApp, const std::string& tag,
		const std::vector<unsigned int>& indices)
	{
		assert(tag != "?");
		return Codex::Resolve<IndexBuffer>(d3dApp, tag, indices);
	}
	std::string IndexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
	std::string IndexBuffer::GetUID() const noexcept
	{
		return GenerateUID_(tag);
	}
}