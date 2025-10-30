// Add the directX 11 header files.
#include <d3d11.h>
#include <DirectXMath.h>

// Define 5 major light types for the light structs.
#define LIGHT_TYPE_DIRECTIONAL		0
#define LIGHT_TYPE_POINT			1
#define LIGHT_TYPE_SPOT				2

class Lights
{
	int type;
	DirectX::XMFLOAT3 direction;
	float range;
	DirectX::XMFLOAT3 position;
	float intensity;
	DirectX::XMFLOAT3 color;
	float spotInnerAngle;
	float spotOuterAngle;
	DirectX::XMFLOAT2 padding;
};

