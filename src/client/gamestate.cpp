#include "client/gamestate.hpp"
using namespace std;

GameState::GameState()
{
	physicsWorld = new Physics(); 
	keyStates = std::unordered_map<int, bool>();

	// Shader lightingShader = Shader("shaders/cube.vertex", "shaders/cube.frag");
	// Shader lightCubeShader = Shader("shaders/light.vertex", "shaders/light.frag"); 
	// Shader modelShader = Shader("shaders/model.vert", "shaders/model.frag");
	
	// shaders.emplace_back(lightingShader);
	// shaders.emplace_back(lightCubeShader);
	// shaders.emplace_back(modelShader);
	// shaders.emplace_back(playerShader);
	// unique_ptr<Shader> playerShader =
	// 	make_unique<Shader>(Shader("../src/client/shaders/shader.vert",
	// 								"../src/client/shaders/shader.frag"));
	// unique_ptr<Model> model = make_unique<Model>(
	// 	Model("../src/client/resources/objects/chicken/Chicken.obj"));
	player = InitializeGameObject("../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag", "../src/client/resources/objects/chicken/Chicken.obj", 
		glm::vec3(0, 5, 0), glm::vec3(0), glm::vec3(0.5), glm::vec3(1));
	// GameObject* cow = new GameObject(this->gameObjects.size(), "../src/client/resources/objects/cow/Cow.obj", false, playerShader);
	// cow->getCollider()->updateCollider(glm::vec3(0), 1.9, 2, 0.9);
	// InitializeGameObject(cow);
}

void GameState::CleanUp() {
    delete physicsWorld;
	for (GameObject *o : gameObjects) {
		delete o;
	}
}

void GameState::Update(float deltaTime) {
	KeyboardInput(deltaTime);
	if (physicsWorld) physicsWorld->Update(deltaTime);
	for (GameObject *o : gameObjects) {
		o->Update(deltaTime);
	}
}

void GameState::Render(const glm::mat4 &viewProjMtx) 
{
  for (GameObject *o : gameObjects) {
    o->Render(viewProjMtx);
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

GameObject* GameState::InitializeGameObject(const char *vertexPath, const char *fragmentPath, const char *modelPath, 
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 collider)
{
	unique_ptr<Shader> shader =
		make_unique<Shader>(Shader(vertexPath, fragmentPath));
	unique_ptr<Model> model = make_unique<Model>(Model(modelPath));
	unique_ptr<Transform> transform_ptr = 
		make_unique<Transform>(Transform(position, rotation, scale));
	unique_ptr<RigidBody> rigidbody_ptr = 
		make_unique<RigidBody>(RigidBody());
	unique_ptr<Collider> collider_ptr = 
		make_unique<Collider>(Collider(position, collider.x, collider.y, collider.z));
	GameObject* object = new GameObject(0, false, transform_ptr, rigidbody_ptr, collider_ptr);
	object->setShader(shader);
	object->setModel(model);
	gameObjects.emplace_back(object);
  	physicsWorld->Add(object);
	return object;
}

void GameState::KeyboardInput(float deltaTime)
{
	// Movement
	glm::vec3 vector = glm::vec3(0);
	float moveSpeed = 4;
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
	if (keyStates[GLFW_KEY_SPACE] == true && player->getRigidBody()->getVelocity().y > -4.905f)
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