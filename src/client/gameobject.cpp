#include "client/gameobject.hpp"

GameObject::GameObject(const std::string &objectId, bool canInteract, Shader sd)
    : id(objectId), interactable(canInteract), active(true), transform(new Transform()), shader(sd), model(new Cube())
{
}

GameObject::~GameObject() {}

void GameObject::activate() { active = true; }

void GameObject::deactivate() { active = false; }

bool GameObject::isActive() const { return active; }

const std::string &GameObject::getId() const { return id; }

void GameObject::setId(const std::string &newId) { id = newId; }

void GameObject::Update(Transform* tf, float deltaTime)
{
    model->update(tf);
}

void GameObject::Render(const glm::mat4& viewProjMtx)
{
    model->draw(viewProjMtx, shader);
}
