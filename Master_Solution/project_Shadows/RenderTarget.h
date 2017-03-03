#pragma once

class RenderTarget
{
public:
	// Constructors
	RenderTarget();
	~RenderTarget();

	unsigned int GetShadowMap();

private:

protected:

	// SHADOW MAPPING
	// All items that cast a shadow, stored in render target
	// Render Target variables:
	unsigned int m_fbo;			//Frame Buffer Object
	unsigned int m_fboDepth;	//Texture Handle - Depth

};

/*
SHADOWS TUTORIAL

Texture: 16-bit depth component
Nearest Filtering, clamp to edge address mode
*/
