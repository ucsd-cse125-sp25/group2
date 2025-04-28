#include "client/renderer.hpp"

Renderer::Renderer(Camera* camera)
{
    this->cam = camera;
}

void Renderer::Render(GameObject* o)
{
    if (renderObjects.find(o) != renderObjects.end()) delete renderObjects[o];
    Model* model = new Model(o->getModelPath());
    if (o->isColliding()) model->ChangeColor(glm::vec3(1.0, 0, 0));
    else model->ChangeColor(glm::vec3(0.0, 0.5, 0));
    renderObjects[o] = model;
    model->Draw(cam->GetViewProjectMtx(), o->getModelMatrix(), o->getShader());
    // o->getCollider()->Draw(cam->GetViewProjectMtx(), o->getShader());
}