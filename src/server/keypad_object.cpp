#include "keypad_object.hpp"

bool KeypadObject::checkSequence(const vector<int> &inputSequence) {
  if (inputSequence == correctSequence) {
    unlocked = true;
    return true;
  }
  return false;
}