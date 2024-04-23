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
		GeometryHelper::CreateRectangle(_geometryTexture); // VertexData && Indices
		_vertexBuffer->Create(_geometryTexture->GetVertices());		
		_indexBuffer->Create(_geometryTexture->GetIndices());
	}

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();

private:
	HWND _hwnd;
	shared_ptr<Graphics> _graphics;

private:
	// Geometry(Mesh)

	// ¸®¼Ò½º
	shared_ptr<Geometry<VertexTextureData>> _geometryTexture;
	shared_ptr<Geometry<VertexColorData>> _geometryColor;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	shared_ptr<VertexShader> _vertexShader;
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;
	shared_ptr<PixelShader> _pixelShader;

	//SRV (Shader Resource View)
	shared_ptr<Texture> _texture;

	// SamplerState
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	// BlenderState
	ComPtr<ID3D11BlendState> _blendState = nullptr;

	


private:
	// SRT (Scale Rotation Translation)
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};

