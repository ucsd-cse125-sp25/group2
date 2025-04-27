#include "client/gameobject.hpp"

GameObject::GameObject(const std::string &objectId, bool canInteract, Shader s)
    : id(objectId), interactable(canInteract), active(true), shader(s), tf(new Transform()), rb(new RigidBody())
{
}

GameObject::~GameObject() {}

void GameObject::activate() { active = true; }
void GameObject::deactivate() { active = false; }
bool GameObject::isActive() const { return active; }

bool GameObject::isInteractable() const { return interactable; }
void GameObject::setInteractability(bool canInteract) { interactable = canInteract; }

const std::string &GameObject::getId() const { return id; }

// void GameObject::setId(const std::string &newId) { id = newId; }

glm::mat4 GameObject::getModelMatrix() const { return modelMat; }
Transform* GameObject::getTransform() const { return tf; }
RigidBody* GameObject::getRigidBody() const { return rb; }
Shader& GameObject::getShader() { return shader; }

void GameObject::Update(float deltaTime) 
{ 
  glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), tf->getScale());
  glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(tf->getRotation().x), glm::vec3(1, 0, 0));
  rotMat = glm::rotate(rotMat, glm::radians(tf->getRotation().y), glm::vec3(0, 1, 0));
  rotMat = glm::rotate(rotMat, glm::radians(tf->getRotation().z), glm::vec3(0, 0, 1));
  glm::mat4 transMat = glm::translate(glm::mat4(1.0f), tf->getPosition());
  
  modelMat = transMat * rotMat * scaleMat; 
}
