#pragma once

class Scene;
class Graphics;

class SceneManager
{
public:
	SceneManager(shared_ptr<Graphics> graphics);

	void Init();
	void Update();
	void LoadScene(wstring sceneName);

	void Render();

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();
	shared_ptr<Graphics> _graphics;
	shared_ptr<Scene> _activeScene;
};

