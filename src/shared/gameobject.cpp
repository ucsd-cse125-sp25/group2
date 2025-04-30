#include "shared/gameobject.hpp"

void GameObject::Update(float deltaTime) { model->Update(transform.get()); }

void GameObject::Render(const glm::mat4 &viewProjMtx) {
  isColliding() ? model->ChangeColor(glm::vec3(1, 0, 0))
                : model->ChangeColor(glm::vec3(1.0f, 0.95f, 0.1f));
  model->Draw(viewProjMtx, shader);
}
