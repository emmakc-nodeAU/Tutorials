#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;

out vec4 vColour;

uniform mat4 projectionViewWorldMatrix;
uniform mat4 modelMatrix;

void main()
{
    mat4 PVM = projectionViewWorldMatrix * modelMatrix;
    gl_Position = PVM * Position;
    vColour = Colour;
}