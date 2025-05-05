#include "shared/gameobject.hpp"

void GameObject::update(Transform *tf) { model->Update(tf); }

void GameObject::draw(const glm::mat4 &viewProjMtx) {
  model->Draw(viewProjMtx, shader);
}
