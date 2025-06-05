#include "condition.hpp"

bool PressurePlateCondition::isSatisfied() const {
  auto cl = object->getCollider()[0];
  if (cl->isWithinTrigger()) {
    if (cl->getTriggerObject() == id) {
      cl->setTriggerObject(-1); // Reset trigger object after satisfying condition
      return true;
    }
  }
  return false;
}

bool PressurePlateOffCondition::isSatisfied() const {
  auto cl = object->getCollider()[0];
  return !cl->isWithinTrigger();
}

bool ButtonCondition::isSatisfied() const { return object->hasBeenPressed(); }

bool PinCondition::isSatisfied() const {
  KeypadObject *keypad = dynamic_cast<KeypadObject *>(object);
  if (keypad) {
    return keypad->solved;
  }
  return false;
}