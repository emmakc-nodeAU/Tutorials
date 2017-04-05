#include "RenderTarget.h"
#include <gl_core_4_4.h>
#include <string>
#include <cstdio>

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
}

unsigned int RenderTarget::GetShadowMap()
{
	// CREATE BUFFER -  ShadowMap
	// FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	// DEPTH
	glGenTextures(1, &m_fboDepth);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);

	// SETTINGS: DEPTH
	// Texture uses a 16-bit depth component format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture: Depth attachment, captures depth !COLOUR
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_fboDepth, 0);

	// Texture: Set Colour to none
	glDrawBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	};

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}


