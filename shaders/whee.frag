#version 460
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float time;

in vec2 tex0;
in vec3 pos;
in vec3 norm;
out vec4 fragColor;

void main()
{
    vec3 mask = texture(texture1, vec2(tex0.x + sin(time), tex0.y)).rgb;
    float masked = dot(mask, mask);
    fragColor = vec4(pos.x * masked, pos.y * masked, pos.z * masked, 1.0);
}
