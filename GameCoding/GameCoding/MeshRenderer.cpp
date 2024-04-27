#include "pch.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer), _device(device)
{	
	_texture = make_shared<Texture>(device);
	_texture->Create(L"TeacherAhn.jpg");
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
	// Render
}
