#pragma once
#include <string>


class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	void CreateGeometry();
	void CreateInputLayout();
	
	void CreateVS();
	void CreatePS();

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;

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

private:
	// Geometry(Mesh)
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	vector<int32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// RS
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;


	// SamplerState
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	// BlenderState
	ComPtr<ID3D11BlendState> _blendState = nullptr;

	//SRV (Shader Resource View)
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView2 = nullptr;


private:
	// SRT (Scale Rotation Translation)
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer = nullptr;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};

