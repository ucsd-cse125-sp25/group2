#include "core.hpp"
#include "cube.hpp"
#include "transform.hpp"
#include <memory>
#include <string>

#ifndef OBJECT_HPP
#define OBJECT_HPP

class GameObject
{
protected:
    std::string id;
    bool interactable;

    // change to model class
    Cube *model;
    Transform *transform;
    Shader shader;

    // displayed in view or not
    bool active;

public:
    GameObject(const std::string &objectId = "", bool interactable = false,
               Shader shader = Shader());

    virtual ~GameObject();

    void activate();
    void deactivate();
    bool isActive() const;

    void setInteractability(bool canInteract);
    bool isInteractable() const;

    const std::string &getId() const;

    // probably don't want to change id of objects
    // void setId(const std::string &newId);

    const Cube *getModel() const;
    void setModel(const Cube *newModel);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;

    void Update(Transform *tf, float deltaTime);
    void Render(const glm::mat4 &viewProjMtx);
};

#endif // OBJECT_HPP