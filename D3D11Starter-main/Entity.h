#pragma once
#include "Transform.h"
#include "Mesh.h"
#include <memory>

class Entity
{
public:
	Entity(Mesh mesh);
	~Entity();
	Transform& GetTransform();
	std::shared_ptr<Mesh> GetMesh();
	void Draw();

private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
};

