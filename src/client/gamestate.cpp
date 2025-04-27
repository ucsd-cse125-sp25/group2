#include "client/gamestate.hpp"

GameState::GameState()
{
	physicsWorld = new Physics(); 
	keyStates = std::unordered_map<int, bool>();

	// Shader lightingShader = Shader("shaders/cube.vertex", "shaders/cube.frag");
	// Shader lightCubeShader = Shader("shaders/light.vertex", "shaders/light.frag");
	// Shader modelShader = Shader("shaders/model.vert", "shaders/model.frag");
	Shader playerShader = Shader("../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag");
	// shaders.emplace_back(lightingShader);
	// shaders.emplace_back(lightCubeShader);
	// shaders.emplace_back(modelShader);
	
	player = new GameObject("player", false, playerShader);
	InitializeGameObject(player);
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
		o->Update(deltaTime);
	}
}

GameObject* GameState::getObject(int id)
{
	return this->gameObjects[id];
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

	// Scale
	float value = 1;
	if (keyStates[GLFW_KEY_E] == true)
		value = 2;
	if (keyStates[GLFW_KEY_Q] == true)
		value = 0.5f;
	player->getTransform()->updateScale(value);
}