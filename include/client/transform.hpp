#pragma once

#include "core.hpp"

class Transform
{
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
public:
	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scl = glm::vec3(1, 1, 1));

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

	void updatePosition(glm::vec3 moveInput);
	void updateRotation(glm::vec3 rotationInput);
	void updateScale(float scaleInput);
};
