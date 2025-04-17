#include <memory>
#include <string>
#include "core.hpp"
#include "cube.hpp"
#include "transform.hpp"

#ifndef OBJECT_HPP
#define OBJECT_HPP

class GameObject
{
protected:
    std::string id;
    bool interactable;
    Cube* model;
    Transform* transform;
    Shader shader;

    // displayed in view or not
    bool active;

public:
    GameObject(const std::string &objectId = "", bool interactable = false, Shader shader = Shader());

    virtual ~GameObject();

    virtual void activate();
    virtual void deactivate();
    bool isActive() const;

    const std::string &getId() const;
    void setId(const std::string &newId);

    void Update(Transform* tf, float deltaTime);
    void Render(const glm::mat4& viewProjMtx);
};

#endif // OBJECT_HPP