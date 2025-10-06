#include "Entity.h"
#include <memory>

Entity::Entity()
{
	// Give an entity a nullptr.
	// this->mesh = nullptr;
	// transform = Transform();
}

Entity::Entity(Mesh mesh, std::shared_ptr<Material> material)
{
	// Initialize the mesh.
	this->mesh = std::make_shared<Mesh>(mesh);

	// Set up the transformation of the entity.
	this->transform = Transform();

	// Initialize material.
	this->material = material;
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
	// Set the material input layout, vertex and pixel shader here.
			// Using the new material for shaders set the input laout.
	Graphics::Context->IASetInputLayout(material.get()->GetInputLayout().Get());

	// Set the active vertex and pixel shaders
	//  - Once you start applying different shaders to different objects,
	//    these calls will need to happen multiple times per frame
	//Graphics::Context->VSSetShader(vertexShader.Get(), 0, 0);
	//Graphics::Context->PSSetShader(pixelShader.Get(), 0, 0);
	// Using the material for shaders, set the shaders.
	Graphics::Context->VSSetShader(material.get()->GetVertexShader().Get(), 0, 0);
	Graphics::Context->PSSetShader(material.get()->GetPixelShader().Get(), 0, 0);

	// Call the mesh draw method here.
	mesh->Draw();
}

std::shared_ptr<Material> Entity::GetMaterial()
{
	// TODO: insert return statement here
	return material;
}

void Entity::SetMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

void Entity::SetMaterial(Material material)
{
	// Create a shared pointer for the new material.
	this->material = std::make_shared<Material>(material);
}
