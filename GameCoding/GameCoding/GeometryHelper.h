#pragma once

class GeometryHelper
{
public:
	static void CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color);
};

