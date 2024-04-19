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
	// 후면 버퍼에 그려달라고 요청
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// 전체 배경의 초기 색상을 지정
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 화면 크기 정보 설정
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

	//::memset(&desc, sizeof(desc);


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

	// HRESULT의 생성에 실패하면 에러 메시지 출력
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	// 텍스처
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// 텍스처를 스왑체인의 후면 버퍼에 넣어줌 (렌더링이 될 백버퍼)
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// RTV 발급 
	// RTV를 GPU에 전달(중간 버퍼) -> 스왑체인을 거쳐 화면에서 출력 
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
