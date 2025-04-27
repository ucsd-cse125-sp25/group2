#pragma once

#include "core.hpp"

class Transform
{
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
public:
	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scl = glm::vec3(1, 1, 1)) 
    {
        this->position = pos;
        this->rotation = rot;
        this->scale = scl;
    }

    glm::vec3 getPosition() { return this->position; }
    glm::vec3 getRotation() { return this->rotation; }
    glm::vec3 getScale() { return this->scale; }

    void setPosition(glm::vec3 pos) { this->position = pos; }

	void updatePosition(glm::vec3 moveInput) { this->position += moveInput; }
	void updateRotation(glm::vec3 rotationInput) { this->rotation += rotationInput; }
	void updateScale(float scaleInput) { this->scale = glm::vec3(scaleInput); }
};