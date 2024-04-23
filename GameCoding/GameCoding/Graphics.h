#pragma once

class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	HWND _hwnd;

private:
	// DX
	// Device & SwapChain

	// ComPtr = Com 객체를 할당하는 ptr
	// ComPtr로 할당, 소멸, 레퍼런스 카운트를 자동으로 관리 (스마트 포인터와 유사)

	// 리소스를 생성하는 팩토리
	ComPtr<ID3D11Device> _device = nullptr;

	// 리소스를 렌더링 파이프라인에 전달
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	// DXGI = 느리게 발전하는 기능들을 따로 모아둔 라이브러리
	// SwapChain = 화면을 표시하는 기능
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV(Resource Target View)
	// GPU(중간 버퍼)에 전달할 명세서
	// RTV 발급 -> GPU에 전달 -> Swap Chain(후면 버퍼) -> 화면 출력
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
};

