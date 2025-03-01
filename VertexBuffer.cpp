#include "VertexBuffer.h"
#include "BindableCodex.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(D3DApp& d3dApp, const Dvtx::VertexBuffer& vbuf)
		:
		VertexBuffer(d3dApp, "?", vbuf)
	{}
	VertexBuffer::VertexBuffer(D3DApp& d3dApp, const std::string& tag, const Dvtx::VertexBuffer& vbuf)
		:
		stride((UINT)vbuf.GetLayout().Size()),
		tag(tag)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.SizeBytes());
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.GetData();
		GetDevice(d3dApp)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}

	void VertexBuffer::Bind(D3DApp& d3dApp) noexcept
	{
		const UINT offset = 0u;
		GetContext(d3dApp)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(D3DApp& d3dApp, const std::string& tag,
		const Dvtx::VertexBuffer& vbuf)
	{
		assert(tag != "?");
		return Codex::Resolve<VertexBuffer>(d3dApp, tag, vbuf);
	}
	std::string VertexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
	std::string VertexBuffer::GetUID() const noexcept
	{
		return GenerateUID(tag);
	}
}