#include "server_object_loader.hpp"

unordered_map<int, unique_ptr<GameObject>> ObjectLoader::loadObjects() {
  unordered_map<int, unique_ptr<GameObject>> objects;

  ifstream file(JSON_PATH);
  if (!file.is_open()) {
    cerr << "Failed to open JSON file: " << JSON_PATH << endl;
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

        if (server.contains("halfExtents")) {
          halfExtents = parseVec3(server, "halfExtents", glm::vec3(1.0f));
        }
        if (server.contains("position")) {
          position = parseVec3(server, "position", glm::vec3(0.0f));
        }
        auto cl = make_unique<Collider>(base.transform->getPosition(), position,
                                        halfExtents);

        obj = make_unique<GameObject>(objectId, base.active, base.transform, rb,
                                      cl);

        if (server.contains("interaction")) {
          string interactionStr = server["interaction"].get<string>();
          auto interactionType =
              magic_enum::enum_cast<InteractionType>(interactionStr);

          if (interactionType.has_value()) {
            obj->setInteractability(interactionType.value());
          }
        }
      }

      objects[objectId] = move(obj);
    }
  }

  return objects;
};