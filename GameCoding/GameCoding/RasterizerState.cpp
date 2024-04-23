#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{

}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D11_FILL_SOLID;
	// �ĸ����� �ν��ϸ� ������ X
	desc.CullMode = D3D11_CULL_BACK;
	// ������ vertex�� �ð� �����̸� �ո�(front), �ƴ϶�� �ĸ�(back)
	desc.FrontCounterClockwise = false;


	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
