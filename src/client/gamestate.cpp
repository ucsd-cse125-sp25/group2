#include "client/gamestate.hpp"

using namespace std;

GameState::GameState() {
  keyStates = std::unordered_map<int, bool>();
  transforms = std::unordered_map<GameObject *, Transform *>();

  // Shader lightingShader = Shader("shaders/cube.vertex", "shaders/cube.frag");
  // Shader lightCubeShader = Shader("shaders/light.vertex",
  // "shaders/light.frag"); Shader modelShader = Shader("shaders/model.vert",
  // "shaders/model.frag");
  unique_ptr<Shader> playerShader = make_unique<Shader>(Shader("../src/client/shaders/shader.vert",
                               "../src/client/shaders/shader.frag"));
  unique_ptr<Model> chickenModel = make_unique<Model>(Model("../src/client/resources/objects/chicken/Chicken.obj"));
  // shaders.emplace_back(lightingShader);
  // shaders.emplace_back(lightCubeShader);
  // shaders.emplace_back(modelShader);

  unique_ptr<Transform> transform_ptr = nullptr;
  player = new GameObject(0, false, transform_ptr);
  player->setShader(playerShader);
  player->setModel(chickenModel);

  InitializeGameObject(player);
}

void GameState::Update(float deltaTime) {
  KeyboardInput(deltaTime);
  for (GameObject *o : gameObjects) {
    o->Update(transforms[o], deltaTime);
  }
}

void GameState::Render(const glm::mat4 &viewProjMtx) {
  for (GameObject *o : gameObjects) {
    o->Render(viewProjMtx);
  }
}

void GameState::InitializeGameObject(GameObject *gameObject) {
  gameObjects.emplace_back(player);
  transforms[player] = new Transform();
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
  transforms[player]->updatePosition(vector * moveSpeed * deltaTime);

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
  transforms[player]->updateRotation(vector * rotSpeed * deltaTime);

  // Scale
  float value = 1;
  if (keyStates[GLFW_KEY_E] == true)
    value = 2;
  if (keyStates[GLFW_KEY_Q] == true)
    value = 0.5f;
  transforms[player]->updateScale(value);
}