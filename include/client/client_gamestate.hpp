#pragma once

#include "client_gameobject.hpp"
#include "client_object_loader.hpp"
#include "globals.hpp"

#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <unordered_map>

using namespace std;

class ClientGameState {
private:
  unordered_map<OBJECT_ID, unique_ptr<GameObject>> objectList;
  unordered_map<LevelType, unordered_map<OBJECT_ID, GameObject *>> levelObjects;
  GameObject *player;

  unordered_map<OBJECT_ID, unique_ptr<BaseUI>> notes;

public:
  Gamestate state;

  bool init();

  // update methods
  void advanceLevel(LevelType newLevel);
  void update(OBJECT_ID id, Transform *tf);
  void draw(const glm::mat4 &viewProjMtx, const glm::vec3 &pos);
  void displayNote(OBJECT_ID id) {
    auto note = notes.find(id);
    if (note != notes.end()) {
      note->second->draw();
    } else {
      cerr << "Note with id " << id << " not found" << endl;
    }
  }

  GameObject *getObject(OBJECT_ID id);
  GameObject *getPlayer() { return player; }
  void setPlayer(OBJECT_ID id) { player = getObject(id); }
};