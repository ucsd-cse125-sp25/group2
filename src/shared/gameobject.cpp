#include "shared/gameobject.hpp"


void GameObject::Update(Transform *tf, float deltaTime) { model->Update(tf); }

void GameObject::Render(const glm::mat4 &viewProjMtx) {
  model->Draw(viewProjMtx, shader);
}
