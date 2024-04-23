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

	// ComPtr = Com ��ü�� �Ҵ��ϴ� ptr
	// ComPtr�� �Ҵ�, �Ҹ�, ���۷��� ī��Ʈ�� �ڵ����� ���� (����Ʈ �����Ϳ� ����)

	// ���ҽ��� �����ϴ� ���丮
	ComPtr<ID3D11Device> _device = nullptr;

	// ���ҽ��� ������ ���������ο� ����
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	// DXGI = ������ �����ϴ� ��ɵ��� ���� ��Ƶ� ���̺귯��
	// SwapChain = ȭ���� ǥ���ϴ� ���
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV(Resource Target View)
	// GPU(�߰� ����)�� ������ ����
	// RTV �߱� -> GPU�� ���� -> Swap Chain(�ĸ� ����) -> ȭ�� ���
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
};

