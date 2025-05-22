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
        auto mcl = make_unique<MeshCollider>(
            server.value("meshcollider", "").c_str(), base.transform.get());
        auto cl = std::vector<Collider *>();
        if (server.contains("collider")) {
          cl = loadCollider(base.transform->getPosition(),
                            server.value("collider", "").c_str());
          // std::cout << cl[0]->getCenter().x << " " << cl[0]->getCenter().y <<
          // " " << cl[0]->getCenter().z << std::endl; std::cout <<
          // cl[0]->getHalfExtents().x << " " << cl[0]->getHalfExtents().y << "
          // " << cl[0]->getHalfExtents().z << std::endl;
        }

        obj = make_unique<GameObject>(objectId, base.active, base.transform, rb,
                                      cl, mcl);

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

std::vector<Collider *> ObjectLoader::loadCollider(glm::vec3 objCenter,
                                                   string path) {
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
      glm::vec3 center, halfExtents;
      if (clData.contains("center")) {
        center = objCenter + parseVec3(clData, "center", glm::vec3(0.0f));
      }
      if (clData.contains("halfExtents")) {
        halfExtents = parseVec3(clData, "halfExtents", glm::vec3(1.0f));
      }
      auto cl = new Collider(center, halfExtents);
      colliders.push_back(cl);
    }
  }
  return colliders;
}