R"(
#version 460
uniform mat4 MVP;
uniform float time;
uniform float maxVert;
in vec3 vPos;
in vec4 vCol;
in vec3 vNorm;
in vec2 uv;
out vec4 color;
out vec2 tex;

vec3 hueShift(vec3 color, float hue) {
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hue);
    return vec3(color * cosAngle + cross(k, color) * sin(hue) + k * dot(k, color) * (1.0 - cosAngle));
}

void main()
{
	vec3 color3 = hueShift( vec3(1.0, 0.0, 0.0), (gl_VertexID / maxVert) * radians(360));
    color = vec4(color3, vCol.a);
    tex = uv;
    vec3 newPos = vec3(vPos + vNorm * sin(time * 0.05));
    gl_Position = MVP * vec4(newPos, 1.0);
}
)";
