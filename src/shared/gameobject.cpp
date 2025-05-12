#include "shared/gameobject.hpp"

void GameObject::Update(float deltaTime) { model->Update(transform.get()); }

void GameObject::Render(const glm::mat4 &viewProjMtx) {
  if (active)
    model->Draw(viewProjMtx, shader);
  // getCollider()->draw(viewProjMtx, shader);
}
