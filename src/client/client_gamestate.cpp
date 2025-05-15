#include "client_gamestate.hpp"

bool ClientGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  player = objectList.begin()->second.get();

  int objectId;
  int isActive;
  float posX, posY, posZ;
  float rotX, rotY, rotZ;
  float scaleX, scaleY, scaleZ;
  string modelPath;
  string vertShaderPath, fragShaderPath;

  while (in.read_row(objectId, isActive, posX, posY, posZ, rotX, rotY, rotZ,
                     scaleX, scaleY, scaleZ, modelPath, vertShaderPath,
                     fragShaderPath)) {
    auto tf = make_unique<Transform>(glm::vec3(posX, posY, posZ),
                                     glm::vec3(rotX, rotY, rotZ),
                                     glm::vec3(scaleX, scaleY, scaleZ));
    auto obj = make_unique<GameObject>(objectId, isActive, tf);
    obj->setModel(make_unique<Model>(modelPath.c_str()));
    obj->setShader(
        make_unique<Shader>(vertShaderPath.c_str(), fragShaderPath.c_str()));
    objectList[objectId] = move(obj);
    player = obj.get();
  }
  state = Gamestate::STARTSCREEN;
  return true;
}

void ClientGameState::update(int id, Transform *tf) {
  auto obj = getObject(id);
  if (obj) {
    obj->update(tf);
  }
}

void ClientGameState::draw(const glm::mat4 &viewProjMtx) {
  // Draw all objects
  for (auto it = objectList.begin(); it != objectList.end(); it++) {
    auto &object = it->second;
    object->draw(viewProjMtx);
  }
}

GameObject *ClientGameState::getObject(int id) {
  auto itr = objectList.find(id);
  if (itr != objectList.end()) {
    return itr->second.get();
  }
  cerr << "Object with id " << id << " not found" << endl;
  return nullptr;
}