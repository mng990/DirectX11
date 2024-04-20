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
	// VS �ε�
	CreateVS();
	// ���� (VS�� �Ѱ��ִ� ����ü�� ���̾ƿ�)
	CreateInputLayout();
	// PS �ε�
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
	// �ĸ� ���ۿ� �׷��޶�� ��û
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// ��ü ����� �ʱ� ������ ����
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// ȭ�� ũ�� ���� ����
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// ȭ�� ���
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;

	// Swap Chain Description�� 0���� �ʱ�ȭ �� ���Ҵ�
	ZeroMemory(&desc, sizeof(desc));
	{
		// �ȼ��� ��� ����, ȭ�Ҽ��� ����
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;

		// �ֻ���
		desc.BufferDesc.RefreshRate.Numerator = 60;

		desc.BufferDesc.RefreshRate.Denominator = 1;

		// vertex �Ű����� ���� ����
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// �ȼ� ���� �ɼ�(Count�� �����Ҽ��� ��� ���� ��ȭ) 
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;

		// ������ �ڵ�
		desc.OutputWindow = _hwnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}


	// _device.Get(): device ��ȯ(ComPtr)
	// _device.GetAddressOf(): device�� �ּҰ� ��ȯ (&Comptr) 

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, // �׷���ī�� ����̹� �Ҵ�
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION, // DX11 ���� ����
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	// HRESULT�� ��κ��� COM �ν��Ͻ����� ��ȯ�ϴ� bool Ÿ�� ����
	// HRESULT�� ������ ����(return 0) �� 
	// ���� �޽��� ���
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	// �ؽ�ó
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// �ؽ�ó�� ����ü���� �ĸ� ���ۿ� �־��� (������ �� �����)
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// RTV �߱� 
	// RTV�� GPU�� ����(�߰� ����) -> ���� ü���� ���� ȭ�鿡�� ��� 
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

		// RW�� ������ ��� �����ϴ� �ɼ� (GPU or CPU) 
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// Buffer�� �뵵 ����
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// ���� ������
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));

		// vertex�� ���� �ּ�([CPU <-> RAM])�� Usage([GPU <-> VRAM])�� ���� 
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

		// RW�� ������ ��� �����ϴ� �ɼ� (GPU or CPU) 
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// Buffer�� �뵵 ����
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		// ���� ������
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));

		// index�� ���� �ּ�([CPU <-> RAM])�� Usage([GPU <-> VRAM])�� ���� 
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

	// d3dcompiler.lib���� ����
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
