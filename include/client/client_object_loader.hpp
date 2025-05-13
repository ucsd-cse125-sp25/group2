#pragma once

#include "base_object_loader.hpp"
#include "transform.hpp"

#include <unordered_map>

class ObjectLoader : public BaseObjectLoader
{
public:
    ObjectLoader() : BaseObjectLoader() {};
    unordered_map<int, unique_ptr<GameObject>> loadObjects();

private:
    int id = 0;
};