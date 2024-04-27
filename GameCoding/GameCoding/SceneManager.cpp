#pragma once

#include "pch.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Game.h"
#include "CameraMove.h"


SceneManager::SceneManager(shared_ptr<Graphics> graphics)
	: _graphics(graphics)
{
}

void SceneManager::Init()
{
	if (_activeScene == nullptr) return;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Update()
{
	if (_activeScene == nullptr) return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FixedUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// Resource
	_activeScene = LoadTestScene();
	Init();
}

void SceneManager::Render()
{
}


shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

	{
		shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			camera->GetOrAddTransform();
			camera->AddComponent(make_shared<Camera>());
			scene->AddGameObject(camera);
		}
		{
			camera->AddComponent(make_shared<CameraMove>());
		}
	}
	
	// Monster1
	{
		shared_ptr<GameObject> _gameObject = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		//_gameObject->GetOrAddTransform()->SetPosition(Vec3{1.f, 1.f, 0.f});
		
		{
			_gameObject->GetOrAddTransform();
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			_gameObject->AddComponent(meshRenderer);
			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}
		{
			auto animator = make_shared<Animator>();
			_gameObject->AddComponent(animator);
			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}
		scene->AddGameObject(_gameObject);
	}


	// Monster2
	{
		shared_ptr<GameObject> _gameObject = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		_gameObject->GetOrAddTransform()->SetPosition(Vec3{ 1.f, 1.f, 0.f });

		{
			_gameObject->GetOrAddTransform();
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			_gameObject->AddComponent(meshRenderer);
			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}
		{
			auto animator = make_shared<Animator>();
			_gameObject->AddComponent(animator);
			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}
		scene->AddGameObject(_gameObject);
	}

	return scene;
}
