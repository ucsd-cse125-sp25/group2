#include "keypad_object.hpp"

bool KeypadObject::checkSequence(const vector<int> &inputSequence) {
  if (inputSequence == correctSequence) {
    solved = true;
    return true;
  }
  return false;
}