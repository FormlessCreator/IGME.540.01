#include "Entity.h"
#include <memory>

Transform& Entity::GetTransform()
{
	// TODO: insert return statement here
	return transform;
}

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return std::shared_ptr<Mesh>();
}

void Entity::Draw()
{

}