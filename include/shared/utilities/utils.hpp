#pragma once

#include "packets.hpp"

#include <iostream>

using namespace std;

// debugging methods
void print(const char *str) { cout << str << endl; }

// Debugging
void printObjectPacket(const ObjectPacket &packet) {
  cout << "ObjectPacket: " << endl;
  cout << "id: " << packet.id << endl;
  cout << "position: (" << packet.transform.getPosition().x << ", "
       << packet.transform.getPosition().y << ", "
       << packet.transform.getPosition().z << ")" << endl;
  cout << "rotation: (" << packet.transform.getRotation().x << ", "
       << packet.transform.getRotation().y << ", "
       << packet.transform.getRotation().z << ")" << endl;
  cout << "scale: (" << packet.transform.getScale().x << ", "
       << packet.transform.getScale().y << ", " << packet.transform.getScale().z
       << ")" << endl;
  cout << "active: " << packet.active << endl;
}