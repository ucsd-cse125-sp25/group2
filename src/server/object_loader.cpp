#include "server_object_loader.hpp"

unordered_map<int, unique_ptr<GameObject>> ObjectLoader::loadObjects() {
  unordered_map<int, unique_ptr<GameObject>> objects;

  ifstream file(OBJECT_PATH);
  if (!file.is_open()) {
    cerr << "Failed to open JSON file: " << OBJECT_PATH << endl;
  }

  json objectsData;
  try {
    file >> objectsData;
  } catch (const exception &e) {
    cerr << "JSON parsing error: " << e.what() << endl;
    return objects;
  }

  if (objectsData.contains("objects") && objectsData["objects"].is_array()) {
    for (const auto &objData : objectsData["objects"]) {
      OBJECT_ID objectId = id++;

      BaseObjectData base = createBaseGameObject(objData);
      unique_ptr<GameObject> obj;

      if (objData.contains("server")) {
        InteractionType interactionType;
        glm::vec3 halfExtents;
        glm::vec3 position;

        auto &server = objData["server"];

        auto rb = make_unique<RigidBody>();
        if (server.contains("static")) {
          bool isStatic = server["static"].get<bool>();
          rb->setStatic(isStatic);
        }

        if (server.contains("mass")) {
          float m = server["mass"].get<float>();
          rb->setMass(m);
        }

        auto cl = vector<Collider *>();
        if (server.contains("colliders") && server["colliders"].is_array()) {
          for (const auto &clData : server["colliders"]) {
            glm::vec3 center = glm::vec3(0), halfExtents = glm::vec3(1);
            glm::mat3 orientation = glm::mat3(1);
            if (clData.contains("center")) {
              center = parseVec3(clData, "center", glm::vec3(0.0f));
            }
            if (clData.contains("halfExtents")) {
              halfExtents = parseVec3(clData, "halfExtents", glm::vec3(1.0f));
            }
            if (clData.contains("orientation")) {
              auto &t = clData["orientation"];
              orientation[0] = parseVec3(t, "right", glm::vec3(1, 0, 0));
              orientation[1] = parseVec3(t, "up", glm::vec3(0, 1, 0));
              orientation[2] = parseVec3(t, "forward", glm::vec3(0, 0, -1));
            }
            auto collider = new Collider(center, halfExtents, orientation);
            if (clData.contains("isTrigger")) {
              collider->setTrigger(clData["isTrigger"].get<bool>());
            }
            cl.push_back(collider);
          }
        }

        for (Collider *c : cl) {
          // if (server.contains("canActivate"))
          //   c->setCanActivate(server["canActivate"].get<bool>());
          c->update(base.transform.get(), objectId < NUM_PLAYERS);
        }

        if (server.contains("keypad")) {
          vector<int> correctSequence;
          if (server["keypad"].contains("correctSequence") &&
              server["keypad"]["correctSequence"].is_array()) {
            for (const auto &seq : server["keypad"]["correctSequence"]) {
              correctSequence.push_back(seq.get<int>());
            }
          }
          obj = make_unique<KeypadObject>(objectId, base.level, base.active,
                                          base.originalPosition, base.transform,
                                          rb, cl, correctSequence);
        } else {
          obj = make_unique<GameObject>(objectId, base.level, base.active,
                                        base.originalPosition, base.transform,
                                        rb, cl);
        }

        if (server.contains("interaction")) {
          string interactionStr = server["interaction"].get<string>();
          auto interactionType =
              magic_enum::enum_cast<InteractionType>(interactionStr);

          if (interactionType.has_value()) {
            obj->setInteractability(interactionType.value());
          }
          if (interactionType == InteractionType::PRESS) {
            string objName = objData["name"].get<string>();
            auto pressFunc = pressFunctionMap.find(objName);
            if (pressFunc != pressFunctionMap.end()) {
              obj->setPressFunction(pressFunc->second);
            } else {
              cerr << "No press function found for object: " << objName << endl;
            }
          }
        }
      }
      objects[objectId] = move(obj);
    }
  }

  return objects;
};