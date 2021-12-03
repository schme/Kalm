R"(
#version 460
uniform sampler2D texture0;

in vec2 tex0;
out vec4 fragColor;

void main()
{
    fragColor = texture(texture0, tex0);
}

)";
