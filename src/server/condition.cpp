#include "condition.hpp"

bool PressurePlateCondition::isSatisfied() const {
  return object->getCollider()[0]->isWithinTrigger();
}

bool ButtonCondition::isSatisfied() const {
  return object->isActive();
}

bool PinCondition::isSatisfied() const {
  return object->isActive();
}