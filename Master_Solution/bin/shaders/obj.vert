// VERTEX SHADER - USE SHADOW
#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 projectionViewWorldMatrix;
uniform mat4 modelMatrix;

out vec4 colour;
out vec2 vUV;

void main()
{
    
	colour = normal;
	mat4 PVM = projectionViewWorldMatrix * modelMatrix;
	vec4 diffuseColour = texture(diffuse, vUV
	
	gl_Position = PVM * position;
}