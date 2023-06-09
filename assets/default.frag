#version 460
uniform mat4 MVP;
uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec4 color;
in vec2 tex;

out vec4 fragColor;

void main()
{
    fragColor = vec4(0.9, 0.0, 0.9, 1.0);
}
