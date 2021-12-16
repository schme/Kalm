R"(
#version 460
uniform mat4 MV;
uniform mat4 P;
uniform mat4 mNormal;
uniform float time;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec4 vCol;
layout (location = 3) in vec2 vTex0;
layout (location = 4) in vec2 vTex1;
layout (location = 5) in vec2 vTex2;

out vec3 norm;
out vec3 eye;

void main()
{
	eye = normalize( vec3(MV * vec4( vPos, 1.0)));
	norm = normalize( mNormal * vec4(vNorm, 1.0)).xyz;

    gl_Position = P * MV * vec4(vPos, 1.0);
}
)";
