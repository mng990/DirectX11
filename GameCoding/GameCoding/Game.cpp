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
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();

	CreateConstantBuffer();
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
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	// CPU <-> GPU 간 데이터 이동 (핵심!!)
	// 1. constantBuffer 생성
	// 2. constantBuffer + subResource 결합
	// 3. transformData를 subResource로 복사
	// 4. constantBuffer + subResource 결합 해제
	// 5. constantBuffer와 VS 연결

	_graphics->GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	_graphics->GetDeviceContext()->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	_graphics->RenderBegin();

	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		_graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		_graphics->GetDeviceContext()->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		_graphics->GetDeviceContext()->IASetInputLayout(_inputLayout.Get());
		_graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_graphics->GetDeviceContext()->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_graphics->GetDeviceContext()->VSSetConstantBuffers(0,1,_constantBuffer.GetAddressOf());


		// RS
		_graphics->GetDeviceContext()->RSSetState(_rasterizerState.Get());

		// PS
		// Shader 리소스를 PS에 전달
		_graphics->GetDeviceContext()->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_graphics->GetDeviceContext()->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_graphics->GetDeviceContext()->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// OM
		// 인덱스 버퍼를 지원하는 Draw를 시행합니다.
		_graphics->GetDeviceContext()->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);
		_graphics->GetDeviceContext()->DrawIndexed(_indices.size(), 0, 0);
	}

	_graphics->RenderEnd();
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// SemanticName을 기존 COLOR에서 TEXCOORD로 수정합니다
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	_graphics->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);
	HRESULT hr = _graphics->GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _graphics->GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
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

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"TeacherAhn.jpg", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	hr = ::LoadFromWICFile(L"Rathalos.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView2.GetAddressOf());
	CHECK(hr);
}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	
	// GPU Read || CPU Write
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 접근 가능


	HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}


void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}
