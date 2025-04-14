#include <memory>
#include <string>

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object
{
protected:
    std::string id;
    bool interactable;
    float positionX;
    float positionY;
    float positionZ;

    // displayed in view or not
    bool active;

public:
    Object(const std::string &objectId = "", bool interactable, float x = 0.0f, float y = 0.0f,
           float z = 0.0f);

    virtual ~Object();

    virtual void activate();
    virtual void deactivate();
    bool isActive() const;

    void setPosition(float x, float y, float z);
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;

    const std::string &getId() const;
    void setId(const std::string &newId);
};

#endif // OBJECT_HPP