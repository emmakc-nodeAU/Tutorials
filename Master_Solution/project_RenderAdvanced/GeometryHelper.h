#pragma once
#include <glm/vec4.hpp>

class RenderData;

namespace GeometryHelper
{

	struct SimpleVertex
	{
		glm::vec4 position;
		glm::vec4 colour;
	};

	RenderData* CreateGrid(unsigned int rows, unsigned int cols, float width, float height, glm::vec4 colour);
	RenderData* CreateQuad(float width, float height, glm::vec3 facingDirection);
};
