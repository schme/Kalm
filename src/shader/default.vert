R"(
#version 460
uniform mat4 MVP;
uniform float time;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec4 vCol;
layout (location = 3) in vec2 vTex0;
layout (location = 4) in vec2 vTex1;
layout (location = 5) in vec2 vTex2;

out vec4 color;
out vec2 tex;

void main()
{
    color = vCol;
    tex = vTex0;
    gl_Position = MVP * vec4(vPos, 1.0);
}
)";
