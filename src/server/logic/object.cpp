#include "server/logic/object.hpp"

Object::Object(const std::string &objectId, bool canInteract, float x, float y, float z)
    : id(objectId), interactable(canInteract), positionX(x), positionY(y), positionZ(z),
      active(true)
{
}

Object::~Object() {}

void Object::activate() { active = true; }

void Object::deactivate() { active = false; }

bool Object::isActive() const { return active; }

void Object::setPosition(float x, float y, float z)
{
    positionX = x;
    positionY = y;
    positionZ = z;
}

float Object::getPositionX() const { return positionX; }

float Object::getPositionY() const { return positionY; }

float Object::getPositionZ() const { return positionZ; }

const std::string &Object::getId() const { return id; }

void Object::setId(const std::string &newId) { id = newId; }
