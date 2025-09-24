#include "Entity.h"
#include <memory>

Entity::Entity(Mesh mesh)
{
	// Initialize the mesh.
	this->mesh = std::make_shared<Mesh>(mesh);

	// Set up the transformation of the mesh.
	transform = 
}

Entity::~Entity()
{

}

Transform& Entity::GetTransform()
{
	// TODO: insert return statement here
	return transform;
}

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

void Entity::Draw()
{
	// Call the mesh draw method here.
	mesh->Draw();
}