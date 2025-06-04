#include "client_object_loader.hpp"

unordered_map<OBJECT_ID, unique_ptr<GameObject>> ObjectLoader::loadObjects() {
  unordered_map<OBJECT_ID, unique_ptr<GameObject>> objects;

  // Open and parse JSON file
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
      OBJECT_ID objectID = id++;

      BaseObjectData base = createBaseGameObject(objData);
      unique_ptr<GameObject> obj =
          make_unique<GameObject>(objectID, base.level, base.active,
                                  base.originalPosition, base.transform);

      if (objData.contains("client")) {
        auto &client = objData["client"];
        auto modelPath = client.value("modelPath", "");
        auto vertShaderPath = client.value("vertShaderPath", "");
        auto fragShaderPath = client.value("fragShaderPath", "");

        obj->setModel(make_unique<Model>(modelPath.c_str()));
        obj->setShader(make_unique<Shader>(vertShaderPath.c_str(),
                                           fragShaderPath.c_str()));
      }
      obj->getModel()->update(
          obj->getTransform()); // Update model with the transform for initial
                                // rendering
      objects[objectID] = move(obj);
    }
  }

  return objects;
};

unordered_map<OBJECT_ID, unique_ptr<BaseUI>> ObjectLoader::loadNotes() {
  unordered_map<OBJECT_ID, unique_ptr<BaseUI>> notes;

  // Open and parse JSON file
  ifstream file(NOTE_PATH);
  if (!file.is_open()) {
    cerr << "Failed to open JSON file: " << NOTE_PATH << endl;
  }

  json notesData;
  try {
    file >> notesData;
  } catch (const exception &e) {
    cerr << "JSON parsing error: " << e.what() << endl;
    return notes;
  }

  if (notesData.contains("notes") && notesData["notes"].is_array()) {
    for (const auto &noteData : notesData["notes"]) {
      OBJECT_ID noteId = noteData.value("id", 0);
      string path = noteData.value("path", "");
      notes[noteId] =
          UIManager::createUIElement(0.0f, 0.0f, 0.25f, 0.5f, 1, nullopt,
                                     path.c_str(), nullptr, false, false);
    }
  }

  return notes;
}

void ObjectLoader::loadLights() {
  // Open and parse JSON file
  ifstream file(LIGHT_PATH);
  if (!file.is_open()) {
    cerr << "Failed to open JSON file: " << LIGHT_PATH << endl;
    return;
  }

  json lightsData;
  try {
    file >> lightsData;
  } catch (const exception &e) {
    cerr << "JSON parsing error: " << e.what() << endl;
    return;
  }

  if (lightsData.contains("lights") && lightsData["lights"].is_array()) {
    for (const auto &lightData : lightsData["lights"]) {
      const LEVEL_ID level = static_cast<LEVEL_ID>(lightData.value("level", -1));
      glm::vec3 position = parseVec3(lightData, "position", glm::vec3(0.0f));
      glm::vec3 color = parseVec3(lightData, "color", glm::vec3(1.0f, 1.0f, 1.0f));
      float radius = lightData.value("radius", 1.0f);
      bool useAttenuation = lightData.value("useAttenuation", true);
      LightManager::addLight(level, position, color, radius, useAttenuation);
    }
  }
}