#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <string>

using namespace std;

void cube2Press();

// Name of object to press function
inline map<string, function<void()>> pressFunctionMap = {
    {"Cube 2", cube2Press}};