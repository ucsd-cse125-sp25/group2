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
      OBJECT_ID objectId = id++;

      BaseObjectData base = createBaseGameObject(objData);
      unique_ptr<GameObject> obj =
          make_unique<GameObject>(objectId, base.active, base.transform);

      if (objData.contains("client")) {
        auto &client = objData["client"];
        auto modelPath = client.value("modelPath", "");
        auto vertShaderPath = client.value("vertShaderPath", "");
        auto fragShaderPath = client.value("fragShaderPath", "");

// If MacOS - turn everything into cube
#ifdef __APPLE__
        modelPath = "../resources/objects/cube/Cube.obj";
        vertShaderPath = "../resources/shaders/shader.vert";
        fragShaderPath = "../resources/shaders/shader.frag";
#endif
        obj->setModel(make_unique<Model>(modelPath.c_str()));
        obj->setShader(make_unique<Shader>(vertShaderPath.c_str(),
                                           fragShaderPath.c_str()));
      }
      obj->getModel()->update(obj->getTransform()); // Update model with the transform for initial rendering
      objects[objectId] = move(obj);
    }
  }

  return objects;
};
