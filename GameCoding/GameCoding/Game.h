#pragma once


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
	void CreateGeometry()
	{
		// VertexData
		{
			_vertices.resize(4);

			_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
			_vertices[0].uv = Vec2(0.f, 1.f);

			_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
			_vertices[1].uv = Vec2(0.f, 0.f);

			_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
			_vertices[2].uv = Vec2(1.f, 1.f);

			_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
			_vertices[3].uv = Vec2(1.f, 0.f);
		}

		// VertexBuffer
		{
			_vertexBuffer->Create(_vertices);
		}

		// Index
		{
			_indices = { 0, 1, 2, 2, 1, 3 };
		}

		// IndexBuffer
		{
			_indexBuffer->Create(_indices);
		}

	}
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
	shared_ptr<Graphics> _graphics;

private:
	// Geometry(Mesh)
	vector<Vertex> _vertices;
	shared_ptr<VertexBuffer> _vertexBuffer;

	//ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	vector<uint32> _indices;
	
	shared_ptr<IndexBuffer> _indexBuffer;
	//ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

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

