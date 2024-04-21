#pragma once
#include "Types.h"

struct Vertex
{
	Vec3 position;
	Vec2 uv; // 입힐 텍스쳐의 비율
};

struct TransformData
{
	Vec3 offset; // 월드 기준의 위치값
	float dummy;
}; 