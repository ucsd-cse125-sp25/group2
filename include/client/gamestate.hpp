#pragma once

#include <vector>
#include <unordered_map>
#include "shader.hpp"
#include "client/gameobject.hpp"
#include "shared/physics.hpp"

class GameState
{
public:
	std::unordered_map<int, bool> keyStates;

	GameState();
	void Update(float deltaTime);

	GameObject* getObject(int id);

	void InitializeGameObject(GameObject* gameObject);
	void KeyboardInput(float deltaTime);

	void CleanUp();

private:
	Physics* physicsWorld;
	std::vector<GameObject*> gameObjects;
	GameObject* player;
};

