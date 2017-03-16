// FRAGMENT SHADER - USE SHADOW
#version 410

in vec4 vNormal;

out vec4 fragColour;

void main()
{
    float d = max(0, dot(normalize(vNormal.xyz), vec3(0,1,0)));
	fragColour = vec4(d,d,d,1);
}

/*
Definitions:
vNormal = Bunny vertex normal

// BELOW For Shadow tutorial:
lightDir = holds a vec3 (vec3(0,1,0)
- Points to Direction of light origin
- Used by ShadowMap
*/