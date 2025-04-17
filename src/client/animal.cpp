#include "client/animal.hpp"

Animal::Animal(float x, float y, float z, const std::string &type)
    : positionX(x), positionY(y), positionZ(z), animalType(type)
{
}

Animal::~Animal() {}

void Animal::move(float deltaX, float deltaY, float deltaZ)
{
    positionX += deltaX;
    positionY += deltaY;
    positionZ += deltaZ;
}

void Animal::interact(const GameObject &other) {}

float Animal::getPositionX() const { return positionX; }

float Animal::getPositionY() const { return positionY; }

float Animal::getPositionZ() const { return positionZ; }

std::string Animal::getAnimalType() const { return animalType; }
