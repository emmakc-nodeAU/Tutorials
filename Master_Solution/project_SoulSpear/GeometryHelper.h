#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

class RenderData;

namespace GeometryHelper
{

	struct SimpleVertex
	{
		glm::vec4 position;
		glm::vec4 colour;
	};

	// TinyOBJLoader
	struct ObjVertex
	{
		glm::vec4 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	RenderData* CreateGrid(unsigned int rows, unsigned int cols, float width, float height, glm::vec4 colour);
	RenderData* CreateQuad(float width, float height, glm::vec3 facingDirection);

	// Multiple Objects: Vector data pointers
	std::vector<RenderData*> LoadOBJFromDisk(std::string path);
};
