#version 460
uniform sampler2D texture0;

in vec2 tex0;
out vec4 fragColor;

void main()
{
	vec3 color = texture(texture0, tex0).rgb;
    fragColor = vec4(color, 1.0);
}
