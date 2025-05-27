#include "condition.hpp"

bool PressurePlateCondition::isSatified() const {
    return object->getCollider()[0]->isWithinTrigger();
}