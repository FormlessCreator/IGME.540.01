#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The local position of the vertex.
	DirectX::XMFLOAT2 uv;           // The UV(x,y) texture coordinate.
	DirectX::XMFLOAT3 normal;		// The normal direction of the surface of the vertex.

	// Create a padding.

	// Add a tangent for the vertex
	DirectX::XMFLOAT3 Tangent;

	// Remove color as materials contain color tint information.
	// DirectX::XMFLOAT4 Color;        // The color of the vertex
};