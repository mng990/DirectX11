#pragma once
#include "Types.h"

struct Vertex
{
	Vec3 position;
	Vec2 uv; // 입힐 텍스쳐의 비율
};

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
}; 