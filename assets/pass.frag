#version 460
uniform sampler2D texture0;

in vec2 tex0;
out vec4 fragColor;

void main()
{
	vec3 color = texture(texture0, tex0).rgb;
	color = pow(color, vec3(1.0/2.2));
    fragColor = vec4(color, 1.0);
}
