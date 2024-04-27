#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{

}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	// 카메라가 위치하는 좌표
	Vec3 eyePosition = GetTransform()->GetPosition();
	// 카메라가 바라보는 방향
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	// 카메라의 y 좌표(수직)
	Vec3 upDirection = GetTransform()->GetUp();
	
	// 위치 좌표, 바라보는 방향, orthogonal vector를 기반으로 계산
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	// 카메라의 월드 좌표를 로컬로 변환 -> 뷰 스페이스
	// S_MatView = GetTransform()->GetWorldMatrix().Invert();

	if (_type == ProjectionType::Perspective)
	{
		// 카메라의 촬영 영역
		// (pi, 화면비, 전면부(near), 후면부(far))
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
	}
	else
	{
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
	}
}
