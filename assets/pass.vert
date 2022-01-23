#version 460
layout (location = 0) in vec2 vPos;
layout (location = 3) in vec2 vTex0;

out vec2 tex0;

void main()
{
	tex0 = vTex0;
	gl_Position = vec4(vPos, 0.0, 1.0);
}
