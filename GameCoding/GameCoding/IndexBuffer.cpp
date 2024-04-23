#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

IndexBuffer::~IndexBuffer()
{
}
