#include "client_gameobject.hpp"

void GameObject::update(Transform *tf) {
  // update the transform of the object
  transform->setPosition(tf->getPosition());
  transform->setRotation(tf->getRotation());
  transform->setScale(tf->getScale());
  // update the transform of the model
  model->update(tf);
}

void GameObject::draw(const glm::mat4 &viewProjMtx, const glm::vec3& pos) {
  model->draw(viewProjMtx, pos, shader);
}