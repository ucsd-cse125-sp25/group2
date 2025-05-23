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
      int objectId = id++;

      BaseObjectData base = createBaseGameObject(objData);
      unique_ptr<GameObject> obj;

      if (objData.contains("server")) {
        InteractionType interactionType;
        glm::vec3 halfExtents;
        auto &server = objData["server"];

        auto rb = make_unique<RigidBody>();
        if (server.contains("static")) {
          bool isStatic = server["static"].get<bool>();
          rb->setStatic(isStatic);
        }
        auto cl = std::vector<Collider *>();
        if (server.contains("collider")) {
          cl = loadCollider(server.value("collider", "").c_str());
        }
        for (Collider *c : cl) {
          if (server.contains("isTrigger"))
            c->setTrigger(server["isTrigger"].get<bool>());
          if (server.contains("canActivate"))
            c->setCanActivate(server["canActivate"].get<bool>());
          c->update(base.transform.get());
        }
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

std::vector<Collider *> ObjectLoader::loadCollider(string path) {
  std::vector<Collider *> colliders;
  ifstream colliderFile(path);
  if (!colliderFile.is_open()) {
    cerr << "Failed to open collider file: " << path << endl;
  }

  json colliderData;
  try {
    colliderFile >> colliderData;
  } catch (const exception &e) {
    cerr << "Collider file parsing error: " << e.what() << endl;
    return colliders;
  }

  if (colliderData.contains("colliders") &&
      colliderData["colliders"].is_array()) {
    for (const auto &clData : colliderData["colliders"]) {
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
        orientation[2] = parseVec3(t, "forward", glm::vec3(0, 0, 1));
      }
      auto cl = new Collider(center, halfExtents, orientation);
      colliders.push_back(cl);
    }
  }
  return colliders;
}