#pragma once

#include "include/common.h"
#include "include/maths.h"
#include "render/RenderTypes.h"

namespace ks::render {

typedef void (*apiProc)(void);
typedef apiProc (*apiLoadFunc)(const char *name);

enum class PolygonMode {
	Fill,
	Line,
};

enum VertexAttribLocation {
    Position = 0,
    Normal,
    Color,
    Texcoord0,
    Texcoord1,
    Texcoord2,
};

void setupEnvironment(apiLoadFunc loadFunc);
void setupFrame(int fbWidth, int fbHeight);

void setPolygonMode(PolygonMode mode);
void setViewport(int x, int y, int width, int height);

u32 generateVAO();
void bindVAO(u32 id);

u32 generateFrameBuffer();
void bindFrameBuffer(u32 id);
void deleteFrameBuffer(u32 fboId, u32 colorTextureId, u32 stencilDepthBufferId);
void setFramebufferAttachment(u32 texture);
bool checkFramebufferStatus();

u32 generateTexture();
void bindTexture(u32 id);
void loadTexture(u32 id, int width, int height, int channels, void *data, bool mipmap=true);

}
