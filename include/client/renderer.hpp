#pragma once

#include <vector>
#include <unordered_map>
#include "shader.hpp"
#include "client/camera.hpp"
#include "shared/gameobject.hpp"
#include "client/model.hpp"

class Renderer
{
public:
	Renderer(Camera* camera);
	void Render(GameObject* o);

private:
    Camera* cam;
	std::unordered_map<GameObject*, Model*> renderObjects;
};

