#ifndef PRESS_FUNCTIONS_HPP
#define PRESS_FUNCTIONS_HPP

#include <iostream>
#include <map>
#include <string>
#include <functional>

using namespace std;

void cube2Press();

// Name of object to press function
inline map<string, function<void()>> pressFunctionMap = {
    {"Cube 2", cube2Press}
};

#endif // PRESS_FUNCTIONS_HPP