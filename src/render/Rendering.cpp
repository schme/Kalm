#include "Rendering.h"

#include "glad/gl.h"

#if 1
#include "glRendering.h"
#endif

namespace ks::render {

void setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void setPolygonMode(PolygonMode mode)
{
	switch(mode)
	{
	case PolygonMode::Fill: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	case PolygonMode::Line: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	}
}

u32 generateVAO()
{
    u32 id;
    glGenVertexArrays(1, &id);
    return id;
}

void bindVAO(u32 id)
{
    glBindVertexArray(id);
}

u32 generateFrameBuffer()
{
    u32 id;
    glGenFramebuffers(1, &id);
    return id;
}
void bindFrameBuffer(u32 id)
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void deleteFrameBuffer(u32 fboId, u32 colorTextureId, u32 stencilDepthBufferId)
{
	glDeleteFramebuffers(1, &fboId);
	glDeleteTextures(1, &colorTextureId);
	glDeleteRenderbuffers(1, &stencilDepthBufferId);
}

u32 generateTexture()
{
    u32 id;
    glGenTextures(1, &id);
    return id;
}

void bindTexture(u32 id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void loadTexture(u32 id, int width, int height, int channels, void *data, bool mipmap)
{
    u32 format = 0;
    switch (channels) {
        case 4:
            format = GL_RGBA;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 2:
            format = GL_RG;
            break;
        case 1:
            format = GL_RED;
            break;
        default:
            format = GL_RGBA;
            break;
    }

    bindTexture(id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

	bindTexture(0);
}

void setFramebufferAttachment(u32 textureId)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
}

bool checkFramebufferStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		log_error("%s", "Framebuffer not complete!");
		return false;
	}
	return true;
}


void setupFrame(int fbWidth, int fbHeight)
{
	static const math::vec4 clearColor = math::vec4(0.f, 0.f, 0.f, 1.f);

	setViewport(0, 0, fbWidth, fbHeight);
	clearColorAndDepthBuffer(clearColor);
}

void setupEnvironment(GLADloadfunc loadFunc)
{
	gladLoadGL(loadFunc);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);

	glDebugMessageCallback( openGlMessageCallback, 0);
}

}
