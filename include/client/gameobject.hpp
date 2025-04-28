#include "client/core.hpp"
#include "shared/cube.hpp"
// #include "client/model.hpp"
#include "client/collider.hpp"
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
    int id;
    const char* modelPath;
    bool interactable;

    glm::mat4 modelMat;
    Transform* tf;
    RigidBody* rb;
    Collider* collider;
    Shader shader;

    // displayed in view or not
    bool active;
    bool colliding;

public:
    GameObject(const int &objectId = -1, const char* path = "", bool interactable = false, Shader s = Shader(), glm::vec3 position = glm::vec3(0));

    virtual ~GameObject();

    void activate();
    void deactivate();
    bool isActive() const;

    void setColliding(bool col);
    bool isColliding() const;

    void setInteractability(bool canInteract);
    bool isInteractable() const;

    const int &getId() const;

    // probably don't want to change id of objects
    // void setId(const std::string &newId);

    const Cube *getModel() const;
    void setModel(const Cube *newModel);

    glm::mat4 getModelMatrix() const;
    Transform* getTransform() const;
    RigidBody* getRigidBody() const;
    Collider* getCollider() const;
    Shader& getShader();
    const char* getModelPath();

    void Update();
};

#endif // OBJECT_HPP