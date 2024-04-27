#pragma once
#include "Component.h"

class Transform : public Component
{
	using Super = Component;
public:
	Transform();
	~Transform();

	virtual void Awake() override;
	virtual void Update() override;

	void UpdateTransform();
	
	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform(); }
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp()	{ return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	// World
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& worldScale);
	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& worldRotation);
	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& worldPosition);

	Matrix GetWorldMatrix() { return _matWorld; }

	// 계층 관계
	bool HasParent() { return _parent != nullptr; }
	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	// Cache
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;

	Vec3 _scale;

	// rotation을 vec로 관리할 경우 gimbal lock 발생
	// -> Quaternion 자료형으로 변환해 관리
	Vec3 _rotation; 
	Vec3 _position;

private:
	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};

