#include "gameobject.hpp"
#include <string>

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

class Animal {
protected:
  float positionX;
  float positionY;
  float positionZ;
  std::string animalType;

public:
  Animal(float x, float y, float z, const std::string &type);

  virtual ~Animal();

  virtual void move(float deltaX, float deltaY, float deltaZ);
  virtual void interact(const GameObject &other);

  float getPositionX() const;
  float getPositionY() const;
  float getPositionZ() const;
  std::string getAnimalType() const;
};

#endif // ANIMAL_HPP