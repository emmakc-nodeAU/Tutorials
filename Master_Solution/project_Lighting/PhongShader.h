//////////////////////////////////////////////////////////////////////
//
//	Name:		Emma Cameron
//	Date:		3 March 2017
//	Project:	Week 4: Lighting
//	Class:		Year 2 Programming, with Matt Dodd
//	
//////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <GLFW\glfw3.h>

#include "Application3D.h"


class Camera;

// 1. CREATE: VERTEX BUFFER
struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class PhongShader : public Application3D {
private:

public:

	PhongShader();
	virtual ~PhongShader();

	// FUNCTION 1. SETUP
	virtual bool startup();
	// FUNCTION 2. SHUTDOWN
	virtual void shutdown();
	// FUNCTION 3. UPDATE
	virtual void update(float deltaTime);
	// FUNCTION 4. DRAW / RENDER
	virtual void draw();


protected:
	// FUNCTION 5. CREATE: SHADER
	void setupShader();

	// FUNCTION 6. CREATE: QUAD
	void generateTexturedQuad();

	// FUNCTION 7. CREATE: BUNNY
	void generateTexturedBunny();

	// VARIABLES: VIRTUAL CAMERA 
	// 1. VIEW MATRIX
	glm::mat4	m_viewMatrix;
	// 2. PROJECTION MATRIX (orthographic, not perspective)
	glm::mat4	m_projectionMatrix;
	// 3. WORLD MATRIX
	glm::mat4	m_worldMatrix;

	// BUFFER VARIABLES: (Remember to clean these up after)
	// VARIABLES: VERTEX ARRAY OBJECT, VERTEX BUFFER OBJ, INDEX BUFFER OBJ
	unsigned int m_VAO;	// Holds vertex position, texture coord data
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_numberOfIndices; // not used anywhere, couldnt keep up with tut

									// Rendering Shader Texture
	unsigned int m_programID;
};


/*
	LIGHTING MODEL: Phong
	PHONG INFORMATION: Colouring a Pixel
	Phong equation: Ambient + Diffuse + Specular = Phong reflection

	PROPERTY VALUES
	1. SURFACE
	- Material Colours/Properties
		A. ambient (kA) Red // uniform vec3 kA = vec3(1,0,0);
		D. diffuse (kD) Red // uniform vec3 kD = vec3(1,0,0);
		S. specular(kS) Red // uniform vec3 kS = vec3(1,0,0);

	- Normal: Light emission direction from SURFACE FACES
		N = normal Unit Vector3 // in vec3 N;

	- View Direction
		V = Ray from SURFACE to CAMERA
		camPos = World-space CAMERA position // uniform vec3 camPos;

	- World Space position
		P = world-space SURFACE position from mesh // in vec3 P;

	2. LIGHT SOURCE
	- Material Colours
		A. ambient (iA)	Grey	// unifrom vec3 iA = vec3(0.25f, 0.25f,0.25f);
		D. diffuse (iD)	White	// uniform vec3 iD = vec3(1,1,1);
		S. specular(iS)			// unifrom vec3 iS = vec3(1,1,1);

	- Normal: Light emission direction from LIGHT SOURCE
		L = normal unit vector3 // uniform vec3 L;

	3. Specular highlights (kS(RM . V)^a iS) (Using GLSL)
 	- Specular power
		- ^a = Reflection sharpness, the shine dot on an object, increase for small pointy dot.
		- SpecularColour = kS*iS^a
		- SpecularTerm = (RM.V)
		// uniform float iSpecPower = 32.0f;

	- Light Direction
		Lm = Incident ray
	- Light Reflection
		Rm = Reflect ray
	- View Direction
		V = Ray from Surface to Camera

	4. ADS Shader elements combined in void main(){}
	AMBIENT:	Ka * Ia = Light colour and Surface Object colour(its own glow?)
	- // vec3 Ambient = kA * iA;

	DIFFUSE:	(Kd * Id) * (Lm . N)  = Scatter of light colour hits object and viewable amount
	- Part 2 // float NdL = max(0.0f, dot (N, -L));					// Lambert Cos Law (Diffuse Term): Surface normal vec dot product and Light vector
	- Part 1 // vec3 Diffuse = kD * iD * NdL;						// Diffuse colour: Light diff * Surface diff

	SPECULAR:	Amount of light hitting object and eye - High spec = v.shiny
	- // vec3 R = reflect(L, N);									// Reflected light vector
	- // vec3 E = normalize( camPos - P);							// SURFACE to SCREEN/Eye vector

	- // float specTerm = pow(min(0.0f, dot (R,E)), iSpecPower);	// Specular Term
	- // vec3 Specular = kS * iS * specTerm;						// Specular Colour/Light
	- // gl_FragColor = vec4(Ambient + Diffuse + Specular, 1);		// Specular Term * Specular Colour

	//////////////////////////////////////////////////////////////////

	1. LOAD MODEL:  Stanford Bunny (Position, Normal)
	2. LIGHT MODEL
	-	BIND Model VertexNormal to GPU
		- Vertex ppty		: vec4 normal
		- Fragment Shader	: update to fake light shining down
*/



