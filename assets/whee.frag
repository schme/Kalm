#version 460
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float time;
uniform vec2 resolution;

in vec2 tex0;
in vec3 pos;
in vec3 norm;
out vec4 fragColor;

#define PI 3.14159265359

float plot(vec2 st, float pct) {
    return smoothstep(pct-0.02, pct, st.y) -
        smoothstep(pct, pct+0.02, st.y);
}

void main()
{
    // Hehe
    vec3 colorA = vec3(0.142, 0.852, 0.643);
    vec3 colorB = vec3(0.642, 0.252, 0.643);

	vec2 st = gl_FragCoord.xy / resolution;
    float y = (sin(st.x * PI * 3 + time) * 0.3) + 0.5;
	vec3 color = mix(colorA, colorB, y);

	float pct = plot(st, y);
	color = (1.0 - pct) * color + pct * vec3(0.0, 0.0, 0.0);

    fragColor = vec4(color, 1.0);
}
