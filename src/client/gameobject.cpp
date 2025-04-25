#include "client/gameobject.hpp"

GameObject::GameObject(const std::string &objectId, bool canInteract, Shader sd)
    : id(objectId), interactable(canInteract), active(true), transform(new Transform()), shader(sd),
      model(new Model("../src/client/resources/objects/chicken/Chicken.obj"))
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

glm::vec3 GameObject::getPosition() const { return transform->getPosition(); }
glm::vec3 GameObject::getRotation() const { return transform->getRotation(); }
glm::vec3 GameObject::getScale() const { return transform->getScale(); }

void GameObject::Update(Transform *tf, float deltaTime) { model->Update(tf); }

void GameObject::Render(const glm::mat4 &viewProjMtx) { model->Draw(viewProjMtx, shader); }
