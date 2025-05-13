#include "client_gamestate.hpp"
#include "client_object_loader.hpp"
#include "client_object_loader.hpp"
#include "globals.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool ClientGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  player = objectList.begin()->second.get();

  return true;
}

void ClientGameState::update(int id, Transform *tf)
{
    auto obj = getObject(id);
    if (obj)
    {
        obj->update(tf);
    }
}

void ClientGameState::draw(const glm::mat4 &viewProjMtx)
{
    // Draw all objects
    for (auto it = objectList.begin(); it != objectList.end(); it++)
    {
        auto &object = it->second;
        object->draw(viewProjMtx);
    }
}

GameObject *ClientGameState::getObject(int id)
{
    auto itr = objectList.find(id);
    if (itr != objectList.end())
    {
        return itr->second.get();
    }
    cerr << "Object with id " << id << " not found" << endl;
    return nullptr;
}