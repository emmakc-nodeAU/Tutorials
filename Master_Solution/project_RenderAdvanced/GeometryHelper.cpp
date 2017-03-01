#include "GeometryHelper.h"
#include <gl_core_4_4.h>
#include "RenderData.h"
#include <glm/glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace GeometryHelper
{
	RenderData * GeometryHelper::CreateGrid(unsigned int rows, unsigned int cols, float width, float height, glm::vec4 colour)
	{
		// VERTEX ARRAY (See Render Geometry tutorial for similar code)
		SimpleVertex* vertices = new SimpleVertex[rows * cols];
		float rowSpacing = width / (cols - 1);
		float colSpacing = height / (rows - 1);

		for (unsigned int r = 0; r < (rows - 1); ++r)
		{
			for (unsigned int c = 0; c < (cols - 1); ++c)
			{
				// Center aligned object on grid axis
				SimpleVertex& vert = vertices[r * cols + c];
				vert.position = glm::vec4(
					-(width / 2) + (colSpacing * c)
					, 0, 
					-(height / 2) + (rowSpacing * r)
					, 1);

				vert.colour = colour;
			}
		}

		// INDEX BUFFER
		unsigned int* indices = new unsigned[(rows - 1) * ( cols - 1) * 6];
		unsigned int index = 0;
		for (unsigned int r = 0; r < (rows - 1); ++r)
		{
			for (unsigned int c = 0; c < (cols - 1); ++c)
			{
				// Tri 1
				indices[index++] = r * cols + c;
				indices[index++] = (r + 1) * cols + c;
				indices[index++] = (r + 1) * cols + (c + 1);

				// Tri 2
				indices[index++] = r * cols + c;
				indices[index++] = (r + 1) * cols + (c +1);
				indices[index++] = r * cols + (c + 1);
			}
		}

		unsigned int numberOfIndices = (rows - 1) * (cols - 1) * 6;

		// RENDER DATA TO CPU
		RenderData* renderData = new RenderData();

		// Generate Buffers - no information in them as yet.
		renderData->GenerateBuffers();

		// Bind  the VAO (automates binding of VBO IBO)- To Push Vertex Data information to Vertex Array of Graphics card
		renderData->Bind();
		// Send vertex data to the VBO
		glBufferData(GL_ARRAY_BUFFER,
			(rows * cols) * sizeof(SimpleVertex), vertices, 
			GL_STATIC_DRAW);
		// Send Index data to the IBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			numberOfIndices * sizeof(unsigned int), indices,
			GL_STATIC_DRAW);

		// Let OpenGL know where to find the data in the vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(SimpleVertex), 
			(char*)offsetof(SimpleVertex, position));

		// ATTRIBUTE: Colour
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(SimpleVertex),
			(char*)offsetof(SimpleVertex, colour));

		renderData->SetNumberofIndices(numberOfIndices);
		renderData->Unbind();

		delete[] vertices;
		delete[] indices;

		return renderData;
	}

	RenderData * GeometryHelper::CreateQuad(float width, float height, glm::vec3 facingDirection)
	{
		return nullptr;
	}
	std::vector<RenderData*> LoadOBJFromDisk(std::string path)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool loadedSuccessfully = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, path.c_str());
		assert(loadedSuccessfully && "Could not load the OBJ file - did you get the path right?");

		std::vector<RenderData*> modelRenderData;

		// For each shape
		for (auto& shape : shapes)
		{
			// Vert List/Table: finds indice for each vert, create new obj vertex
			std::vector<ObjVertex> vertices;
			vertices.reserve(shape.mesh.num_face_vertices.size() * 3);

			int index = 0;
			for (auto& face : shape.mesh.num_face_vertices)
			{
				for (int i = 0; i < 3; ++i)
				{
					tinyobj::index_t idx = shape.mesh.indices[index + i];

					// SETS POSITION
					ObjVertex v;
					v.position.x = attribs.vertices[3 * idx.vertex_index + 0];
					v.position.y = attribs.vertices[3 * idx.vertex_index + 1];
					v.position.z = attribs.vertices[3 * idx.vertex_index + 2];
					v.position.w = 1.0f;

					// if exist - SETS NORMALS
					if (!attribs.normals.empty())
					{
						v.normal.x = attribs.normals[3 * idx.normal_index + 0];
						v.normal.y = attribs.normals[3 * idx.normal_index + 1];
						v.normal.z = attribs.normals[3 * idx.normal_index + 2];
					}

					// if exist - SETS UV COORDS
					if (!attribs.texcoords.empty())
					{
						v.uv.s = attribs.texcoords[2 * idx.texcoord_index + 0];
						v.uv.t = attribs.texcoords[2 * idx.texcoord_index + 1];
					}
				}
				index += face;
			}
			// Put into RenderData
			RenderData* renderData = new RenderData();
			// !IBO
			renderData->GenerateBuffers(false);
			// Pass Vertex Info to GPU
			renderData->Bind();

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ObjVertex), vertices.data(), GL_STATIC_DRAW);

			// Setup Attribuite Points (Position, Normal, UV)
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			// Specify what each slot has
			glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, position));
			glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, normal));
			glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(ObjVertex), (char*)offsetof(ObjVertex, uv));

			// Unbind
			renderData->Unbind();
			modelRenderData.push_back(renderData);
		}

		return modelRenderData;
	}
}

	/* TO DRAW GEOMETRY

	1. Get Infomration
	a. vertex array
	b. index array

	2. Create new render data
	2.a creates buffers
	2.b Bind render data, bind VAO too

	3. Send Verts and Indices across to GPU

	4. Let OPENGL know about the layout of the vertex
	4.1 slot 0 has 4 floats to read position, slout 1 has 4 floats to read colour.

	5. number of indices to read itself later on.

	6. unbind the render data

	7. use render data for anything else.

	8. write render shader...

	9. write lighting...
	
	*/