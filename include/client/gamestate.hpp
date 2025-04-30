#pragma once

#include "client/shader.hpp"
#include "shared/physics.hpp"
#include "shared/gameobject.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

class GameState {
public:
	std::unordered_map<int, bool> keyStates;

	GameState();
	void Update(float deltaTime);
	void Render(const glm::mat4 &viewProjMtx);

	GameObject* getObject(int id);
	std::vector<GameObject*> getAllObjects();
	GameObject* InitializeGameObject(const char *vertexPath, const char *fragmentPath, const char *modelPath, 
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 collider);
	void KeyboardInput(float deltaTime);

	void CleanUp();

private:
	std::vector<GameObject*> gameObjects;
	Physics* physicsWorld;
	GameObject* player;
};
