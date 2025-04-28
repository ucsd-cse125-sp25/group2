#include "client/gamestate.hpp"

GameState::GameState()
{
	physicsWorld = new Physics(); 
	keyStates = std::unordered_map<int, bool>();

	// Shader lightingShader = Shader("shaders/cube.vertex", "shaders/cube.frag");
	// Shader lightCubeShader = Shader("shaders/light.vertex", "shaders/light.frag");
	// Shader modelShader = Shader("../src/client/shaders/model.vert", "../src/client/shaders/model.frag");
	Shader playerShader = Shader("../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag");
	// shaders.emplace_back(lightingShader);
	// shaders.emplace_back(lightCubeShader);
	// shaders.emplace_back(modelShader);
	
	player = new GameObject(this->gameObjects.size(), "../src/client/resources/objects/chicken/Chicken.obj", false, playerShader, glm::vec3(-4, 5, 0));
	player->getTransform()->updateScale(0.5f);
	player->getCollider()->updateCollider(glm::vec3(-4, 5, 0), 1, 1, 1);
	InitializeGameObject(player);
	GameObject* cow = new GameObject(this->gameObjects.size(), "../src/client/resources/objects/cow/Cow.obj", false, playerShader);
	cow->getCollider()->updateCollider(glm::vec3(0), 1.9, 2, 0.9);
	InitializeGameObject(cow);
}

void GameState::CleanUp() {
    delete physicsWorld;
	delete player;
}

void GameState::Update(float deltaTime)
{
	KeyboardInput(deltaTime);
	if (physicsWorld) physicsWorld->Update(deltaTime);
	for (GameObject* o : gameObjects)
	{
		o->Update();
	}
}

GameObject* GameState::getObject(int id)
{
	return this->gameObjects[id];
}

std::vector<GameObject*> GameState::getAllObjects()
{
	return this->gameObjects;
}

void GameState::InitializeGameObject(GameObject* gameObject)
{
	gameObjects.emplace_back(gameObject);
    physicsWorld->Add(gameObject);
}

void GameState::KeyboardInput(float deltaTime)
{
	// Movement
	glm::vec3 vector = glm::vec3(0);
	float moveSpeed = 8;
	if (keyStates[GLFW_KEY_W] == true)
		vector -= glm::vec3(0, 0, 1);
	if (keyStates[GLFW_KEY_S] == true)
		vector += glm::vec3(0, 0, 1);
	if (keyStates[GLFW_KEY_A] == true)
		vector -= glm::vec3(1, 0, 0);
	if (keyStates[GLFW_KEY_D] == true)
		vector += glm::vec3(1, 0, 0);
	if (vector != glm::vec3(0))
		vector = glm::normalize(vector);
	player->getTransform()->updatePosition(vector * moveSpeed * deltaTime);
		

	float jumpForce = 1000.0f;
	if (keyStates[GLFW_KEY_SPACE] == true)
	{
		player->getTransform()->updatePosition(glm::vec3(0, 0.1, 0));
	}
	
	// Rotation
	vector = glm::vec3(0);
	float rotSpeed = 15;
	if (keyStates[GLFW_KEY_UP] == true)
		vector -= glm::vec3(rotSpeed, 0, 0);
	if (keyStates[GLFW_KEY_DOWN] == true)
		vector += glm::vec3(rotSpeed, 0, 0);
	if (keyStates[GLFW_KEY_LEFT] == true)
		vector += glm::vec3(0, rotSpeed, 0);
	if (keyStates[GLFW_KEY_RIGHT] == true)
		vector -= glm::vec3(0, rotSpeed, 0);
	player->getTransform()->updateRotation(vector * rotSpeed * deltaTime);	
}