#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include <memory>

class Entity
{
public:
	Entity();
	Entity(Mesh mesh, std::shared_ptr<Material> material);
	~Entity();
	Transform& GetTransform();
	std::shared_ptr<Mesh> GetMesh();
	void Draw();

	// A Get and set for the material.
	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> material);
	void SetMaterial(Material material);
	

private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;

	// Add a material shared ptr field for the entity.
	std::shared_ptr<Material> material;
};

