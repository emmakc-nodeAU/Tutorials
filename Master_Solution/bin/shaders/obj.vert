// VERTEX SHADER - USE SHADOW
#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 vNormal;
layout(location=2) in vec2 uv;

uniform mat4 projectionViewWorldMatrix;
uniform mat4 modelMatrix;

out vec4 colour;
out vec2 vUV;

void main()
{
    
	colour = vNormal;
	mat4 PVM = projectionViewWorldMatrix * modelMatrix;
	vec4 diffuseColour = texture(colour, vUV);
	
	gl_Position = PVM * position;
}