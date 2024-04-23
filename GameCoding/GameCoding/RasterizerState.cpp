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
	// 후면으로 인식하면 렌더링 X
	desc.CullMode = D3D11_CULL_BACK;
	// 도형의 vertex가 시계 방향이면 앞면(front), 아니라면 후면(back)
	desc.FrontCounterClockwise = false;


	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
