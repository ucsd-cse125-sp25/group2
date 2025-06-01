#include "condition.hpp"

bool PressurePlateCondition::isSatisfied() const {
  auto cl = object->getCollider()[0];
  if (cl->isWithinTrigger())
    return cl->getTriggerObject() == id;
  return false;
}

bool ButtonCondition::isSatisfied() const { return object->isActive(); }

bool PinCondition::isSatisfied() const { return object->isActive(); }