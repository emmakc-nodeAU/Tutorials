#version 410

in vec4 colour;

out vec4 fragColour;

void main()
{
    fragColour = vec4
	(colour, 1);
}