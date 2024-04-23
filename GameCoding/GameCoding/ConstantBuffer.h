#pragma once

// CPU <-> GPU 간 데이터 고속 복사 (핵심!!)
// 1. constantBuffer 생성
// 2. constantBuffer + subResource 결합 (_deviceContext->Map)
// 3. transformData를 subResource로 복사
// 4. constantBuffer + subResource 결합 해제 (_deviceContext->UnMap)
// +) deviceContext를 거쳐 GPU로 데이터를 전달

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
		: _device(device), _deviceContext(deviceContext)
	{
	}

	~ConstantBuffer(){}

	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	void Create() 
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// GPU Read || CPU Write
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 접근 가능


		HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		CHECK(hr);
	}

	void CopyData(const T& data) 
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, &data, sizeof(data));
		_deviceContext->Unmap(_constantBuffer.Get(), 0);
	}


private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

