#pragma once
#include "Types.h"

struct Vertex
{
	Vec3 position;
	Vec2 uv; // ���� �ؽ����� ����
};

struct TransformData
{
	Vec3 offset; // ���� ������ ��ġ��
	float dummy;
}; 