
#include "client_object_loader.hpp"

unordered_map<int, unique_ptr<GameObject>> ObjectLoader::loadObjects() {
  unordered_map<int, unique_ptr<GameObject>> objects;

  // Open and parse JSON file
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
      unique_ptr<GameObject> obj =
          make_unique<GameObject>(objectId, base.active, base.transform);

      if (objData.contains("client")) {
        auto &client = objData["client"];

        obj->setModel(
            make_unique<Model>(client.value("modelPath", "").c_str()));
        obj->setShader(
            make_unique<Shader>(client.value("vertShaderPath", "").c_str(),
                                client.value("fragShaderPath", "").c_str()));
      }

      objects[objectId] = move(obj);
    }
  }

  return objects;
};
