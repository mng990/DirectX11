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

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	// TODO
}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();

	// IA - VS - RS - PS - OM
	{

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
	// [ ] <- [ ]
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

	//::memset(&desc, sizeof(desc);


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

	// HRESULT�� ������ �����ϸ� ���� �޽��� ���
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	// �ؽ�ó
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// �ؽ�ó�� ����ü���� �ĸ� ���ۿ� �־��� (�������� �� �����)
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// RTV �߱� 
	// RTV�� GPU�� ����(�߰� ����) -> ����ü���� ���� ȭ�鿡�� ��� 
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 1.f;
	_viewport.MaxDepth = 1.f;
}
