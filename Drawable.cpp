#include "Drawable.h"
#include "IndexBuffer.h"
#include <cassert>

void Drawable::Draw(D3DApp& d3dApp) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(d3dApp);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(d3dApp);
	}
	d3dApp.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}