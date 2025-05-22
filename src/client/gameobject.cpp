#include "client_gameobject.hpp"

void GameObject::update(Transform *tf) { model->update(tf); }

void GameObject::draw(const glm::mat4 &viewProjMtx) {
  model->draw(viewProjMtx, shader);
}

