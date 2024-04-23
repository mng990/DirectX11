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

	_graphics = make_shared<Graphics>(hwnd);
	_geometryTexture = make_shared<Geometry<VertexTextureData>>();
	_geometryColor = make_shared<Geometry<VertexColorData>>();

	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	
	_texture = make_shared<Texture>(_graphics->GetDevice());
	_samplerState = make_shared<SamplerState>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());

	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());


	GeometryHelper::CreateRectangle(_geometryTexture);
	_vertexBuffer->Create(_geometryTexture->GetVertices());
	_indexBuffer->Create(_geometryTexture->GetIndices());
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());
	_texture->Create(L"TeacherAhn.jpg");
	_samplerState->Create();

	_constantBuffer->Create();
}

void Game::Update()
{
	// SRT(Scale Rotation Translation)

	_localPosition.x += 0.001f;
	_localPosition.y += 0.001f;

	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation; // SRT
	_transformData.matWorld = matWorld;

	// CPU(_transformData) -> GPU로 데이터 복사
	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();

	{
		PipelineInfo info;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.pixelShader = _pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;

		_pipeline->UpdatePipeline(info);

		_pipeline->SetVertexBuffer(_vertexBuffer);
		_pipeline->SetIndexBuffer(_indexBuffer);
		_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
		_pipeline->SetTexture(0, SS_PixelShader, _texture);
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

		_pipeline->DrawIndexed(_geometryTexture->GetIndexCount(), 0, 0);
	}

	_graphics->RenderEnd();
}




