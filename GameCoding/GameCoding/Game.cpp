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
	_width = GWinSizeX;
	_height = GWinSizeY;

	// default setting
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	// VS 로드
	CreateVS();
	// 설명서 (VS로 넘겨주는 구조체의 레이아웃)
	CreateInputLayout();
	// PS 로드
	CreatePS();
}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();

	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		
		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

		// OM
		//_deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();
}

void Game::RenderBegin()
{
	// 후면 버퍼에 그려달라고 요청
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// 전체 배경의 초기 색상을 지정
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 화면 크기 정보 설정
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// 화면 출력
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;

	// Swap Chain Description을 0으로 초기화 후 재할당
	ZeroMemory(&desc, sizeof(desc));
	{
		// 픽셀을 담는 버퍼, 화소수랑 동일
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;

		// 주사율
		desc.BufferDesc.RefreshRate.Numerator = 60;

		desc.BufferDesc.RefreshRate.Denominator = 1;

		// vertex 매개변수 전달 형식
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 픽셀 보간 옵션(Count가 증가할수록 계단 현상 완화) 
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;

		// 윈도우 핸들
		desc.OutputWindow = _hwnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}


	// _device.Get(): device 반환(ComPtr)
	// _device.GetAddressOf(): device의 주소값 반환 (&Comptr) 

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, // 그래픽카드 드라이버 할당
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION, // DX11 세부 버전
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	// HRESULT는 대부분의 COM 인스턴스들이 반환하는 bool 타입 변수
	// HRESULT의 생성에 실패(return 0) 시 
	// 에러 메시지 출력
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	// 텍스처
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// 텍스처를 스왑체인의 후면 버퍼에 넣어줌 (렌더링 될 백버퍼)
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// RTV 발급 
	// RTV를 GPU에 전달(중간 버퍼) -> 스왑 체인을 거쳐 화면에서 출력 
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{
	// VertexData
	{
		_vertices.resize(4);

		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].color = Color(0.5f, 0.f, 1.f, 0.5f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].color = Color(0.f, 1.f, 1.f, 0.5f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].color = Color(0.f, 0.f, 1.f, 0.5f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].color = Color(0.f, 0.f, 1.f, 0.5f);
	}

	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// RW를 시행할 장비를 설정하는 옵션 (GPU or CPU) 
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// Buffer의 용도 설정
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// 버퍼 사이즈
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));

		// vertex의 시작 주소([CPU <-> RAM])를 Usage([GPU <-> VRAM])로 전달 
		data.pSysMem = _vertices.data();

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}


	// IndexData
	{
		_indices = {0, 1, 2, 2, 1, 3};
	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// RW를 시행할 장비를 설정하는 옵션 (GPU or CPU) 
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// Buffer의 용도 설정
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		// 버퍼 사이즈
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));

		// index의 시작 주소([CPU <-> RAM])를 Usage([GPU <-> VRAM])로 전달 
		data.pSysMem = _indices.data();

		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};


	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	HRESULT hr = _device->CreateInputLayout(layout, ARRAYSIZE(layout), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());

	CHECK(hr);
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// d3dcompiler.lib에서 지원
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
