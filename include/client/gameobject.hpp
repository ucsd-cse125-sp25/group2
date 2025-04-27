#include "client/core.hpp"
#include "shared/cube.hpp"
// #include "client/model.hpp"
#include "client/rigidbody.hpp"
#include "client/transform.hpp"
// #include "shared/physics.hpp"
#include <memory>
#include <string>

#ifndef OBJECT_HPP
#define OBJECT_HPP



class GameObject
{
protected:
    std::string id;
    bool interactable;

    // Model *model;
    glm::mat4 modelMat;
    Transform* tf;
    RigidBody* rb;
    Shader shader;

    // displayed in view or not
    bool active;

public:
    GameObject(const std::string &objectId = "", bool interactable = false, Shader s = Shader());

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

    glm::mat4 getModelMatrix() const;
    Transform* getTransform() const;
    RigidBody* getRigidBody() const;
    Shader& getShader();

    void Update(float deltaTime);
};

#endif // OBJECT_HPP