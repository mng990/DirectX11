#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device> device)
	: Super(ResourceType::Mesh), _device(device)
{

}

Mesh::~Mesh()
{

}

void Mesh::CreateDefaultRectangle()
{
	_geometryTexture = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometryTexture);

	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometryTexture->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometryTexture->GetIndices());
}