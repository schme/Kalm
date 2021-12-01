R"(
#version 460
uniform mat4 MVP;
uniform float time;
uniform sampler2D txtr1;
in vec4 color;
in vec2 tex;
out vec4 fragColor;

void main()
{
    fragColor = color * texture(txtr1, tex);
}
)";
