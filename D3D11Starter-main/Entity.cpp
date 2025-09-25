#include "Entity.h"
#include <memory>

Entity::Entity()
{
	// Give an entity a nullptr.
	// this->mesh = nullptr;
	// transform = Transform();
}

Entity::Entity(Mesh mesh)
{
	// Initialize the mesh.
	this->mesh = std::make_shared<Mesh>(mesh);

	// Set up the transformation of the entity.
	this->transform = Transform();
}

Entity::~Entity()
{

}

Transform& Entity::GetTransform()
{
	// TODO: insert return statement here
	return this->transform;
}

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return this->mesh;
}

void Entity::Draw()
{
	// Call the mesh draw method here.
	mesh->Draw();
}