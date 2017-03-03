// FRAGMENT SHADER - USE SHADOW
#version 410

in vec4 vNormal
//in vec4 colour; 

out vec4 fragColour;

uniform vec3 lightDir;

void main()
{
    float d = max(0, dot(normalize(vNormal.xyz), lightDir));
	fragColour = vec4(d,d,d,1);
}

/*
Definitions:
lightDir = holds a vec3
- Points to Direction of light origin
- Used by ShadowMap
*/