R"(
#version 460
uniform mat4 MVP;
uniform float time;
uniform vec3 eye;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec4 vCol;
layout (location = 3) in vec2 vTex0;
layout (location = 4) in vec2 vTex1;
layout (location = 5) in vec2 vTex2;

out vec3 pos;
out vec4 color;
out vec2 tex0;
out vec3 norm;

void main()
{
		pos = vPos;
    color = vCol;
    tex0 = vTex0;
		norm = vNorm;
    vec3 newPos = vec3(vPos + vNorm * sin(time * 0.05));
    gl_Position = MVP * vec4(newPos, 1.0);
}
)";
