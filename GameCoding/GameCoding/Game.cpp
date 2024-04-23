#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;

	_graphics = make_shared<Graphics>(hwnd);

	_geometryTexture = make_shared<Geometry<VertexTextureData>>();
	_geometryColor = make_shared<Geometry<VertexColorData>>();
	
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());
	
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_texture = make_shared<Texture>(_graphics->GetDevice());

	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());


	CreateGeometry();
	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	_texture->Create(L"TeacherAhn.jpg");

	_constantBuffer->Create();
}

void Game::Update()
{
	// SRT(Scale Rotation Translation)

	_localPosition.x += 0.001f;
	_localPosition.y += 0.001f;

	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation; // SRT
	_transformData.matWorld = matWorld;

	// CPU(_transformData) -> GPU로 데이터 복사
	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();

	{
		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;

		// IA
		_graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		_graphics->GetDeviceContext()->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		_graphics->GetDeviceContext()->IASetInputLayout(_inputLayout->GetComPtr().Get());
		_graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_graphics->GetDeviceContext()->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
		_graphics->GetDeviceContext()->VSSetConstantBuffers(0,1,_constantBuffer->GetComPtr().GetAddressOf());


		// RS
		_graphics->GetDeviceContext()->RSSetState(_rasterizerState.Get());

		// PS
		// Shader 리소스를 PS에 전달
		_graphics->GetDeviceContext()->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
		_graphics->GetDeviceContext()->PSSetShaderResources(0, 1, _texture->GetComPtr().GetAddressOf());
		_graphics->GetDeviceContext()->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// OM
		// 인덱스 버퍼를 지원하는 Draw를 시행합니다.
		_graphics->GetDeviceContext()->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);
		_graphics->GetDeviceContext()->DrawIndexed(_geometryTexture->GetIndexCount(), 0, 0);
	}

	_graphics->RenderEnd();
}


void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D11_FILL_SOLID;
	// 후면으로 인식하면 렌더링 X
	desc.CullMode = D3D11_CULL_BACK; 
	// 도형의 vertex가 시계 방향이면 앞면(front), 아니라면 후면(back)
	desc.FrontCounterClockwise = false; 


	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	
	//Sampling을 추출하는 규약들
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.f;

	HRESULT hr = _graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateBlendState()
{
	//desc 생성
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	//desc 옵션 설정
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	// 블렌딩 옵션
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
}



