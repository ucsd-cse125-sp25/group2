#include "client/gamestate.hpp"
using namespace std;

GameState::GameState() {
  physicsWorld = new Physics();
  keyStates = std::unordered_map<int, bool>();

  // Also just using hardcoded collision box sizes
  player = InitializeGameObject(
      "../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag",
      "../src/client/resources/objects/chicken/Chicken.obj", glm::vec3(0, 5, 0),
      glm::vec3(0), glm::vec3(1), glm::vec3(0.235, 0.25, 0.165));
  InitializeGameObject(
      "../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag",
      "../src/client/resources/objects/pig/Pig.obj", glm::vec3(0, 0, 0),
      glm::vec3(0), glm::vec3(1), glm::vec3(0.625, 0.5, 0.218));
  // InitializeGameObject("../src/client/shaders/shader.vert",
  // "../src/client/shaders/shader.frag",
  // "../src/client/resources/objects/sheep/Sheep.obj", 	glm::vec3(0, 0, 0),
  // glm::vec3(0), glm::vec3(1), glm::vec3(0.86, 0.625, 0.565));
  // InitializeGameObject("../src/client/shaders/shader.vert",
  // "../src/client/shaders/shader.frag",
  // "../src/client/resources/objects/cow/Cow.obj",
  //   glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1), glm::vec3(0.955, 0.75,
  //   0.38));
}

void GameState::CleanUp() {
  delete physicsWorld;
  for (GameObject *o : gameObjects) {
    delete o;
  }
}

void GameState::Update(float deltaTime) {
  KeyboardInput(deltaTime);
  if (physicsWorld)
    physicsWorld->Update(deltaTime);
  for (GameObject *o : gameObjects) {
    o->Update(deltaTime);
  }
}

void GameState::Render(const glm::mat4 &viewProjMtx) {
  for (GameObject *o : gameObjects) {
    o->Render(viewProjMtx);
  }
}

GameObject *GameState::getObject(int id) { return this->gameObjects[id]; }

std::vector<GameObject *> GameState::getAllObjects() {
  return this->gameObjects;
}

GameObject *GameState::InitializeGameObject(const char *vertexPath,
                                            const char *fragmentPath,
                                            const char *modelPath,
                                            glm::vec3 position,
                                            glm::vec3 rotation, glm::vec3 scale,
                                            glm::vec3 collider) {
  unique_ptr<Shader> shader =
      make_unique<Shader>(Shader(vertexPath, fragmentPath));
  unique_ptr<Model> model = make_unique<Model>(Model(modelPath));
  unique_ptr<Transform> transform_ptr =
      make_unique<Transform>(Transform(position, rotation, scale));
  unique_ptr<RigidBody> rigidbody_ptr = make_unique<RigidBody>(RigidBody());
  unique_ptr<Collider> collider_ptr = make_unique<Collider>(
      Collider(position, collider.x, collider.y, collider.z));
  GameObject *object = new GameObject(gameObjects.size(), false, transform_ptr,
                                      rigidbody_ptr, collider_ptr);
  object->setShader(shader);
  object->setModel(model);
  // Calculate area for air resistance (Might move this to rigidbody)
  float l = 2 * collider.x;
  float h = 2 * collider.y;
  float w = 2 * collider.z;
  object->setArea(2 * (w * l + h * l + h * w));

  gameObjects.emplace_back(object);
  physicsWorld->Add(object);
  return object;
}

void GameState::KeyboardInput(float deltaTime) {
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

  player->getRigidBody()->applyImpulse(moveSpeed * vector);

  float jumpForce = 10.0f;
  if (keyStates[GLFW_KEY_SPACE] == true && player->isGrounded())
    player->getRigidBody()->applyImpulse(jumpForce * glm::vec3(0, 1, 0));

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