#pragma once
#include "ResourceBase.h"

class Mesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	Mesh(ComPtr<ID3D11Device> device);
	virtual ~Mesh();

	void CreateDefaultRectangle();

	shared_ptr<VertexBuffer> GetVertexBuffer()						{ return _vertexBuffer;	}
	shared_ptr<IndexBuffer> GetIndexBuffer()						{ return _indexBuffer; }
	shared_ptr<Geometry<VertexTextureData>> GetGeometryTexture()	{ return _geometryTexture; }

private:
	
	ComPtr<ID3D11Device> _device;

	// Mesh
	shared_ptr<Geometry<VertexTextureData>> _geometryTexture;
	shared_ptr<Geometry<VertexColorData>> _geometryColor;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

