#pragma once
class GameObject
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Update();
	void Render(shared_ptr<Pipeline> pipeline);
private:
	ComPtr<ID3D11Device> _device;

	// Geometry(Mesh)
	// ¸®¼Ò½º
	shared_ptr<Geometry<VertexTextureData>> _geometryTexture;
	shared_ptr<Geometry<VertexColorData>> _geometryColor;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;
	shared_ptr<RasterizerState> _rasterizerState;

private:
	// SRT (Scale Rotation Translation)
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};

