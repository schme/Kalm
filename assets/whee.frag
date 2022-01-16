#version 460
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float time;
uniform vec2 resolution;

in vec2 tex0;
in vec3 pos;
in vec3 norm;
out vec4 fragColor;

float plot(vec2 st) {
	return smoothstep(0.02 + sin(time), 0.0, abs(st.y - st.x));
}

void main()
{
	vec2 st = gl_FragCoord.xy / resolution;
	float y = st.x;
	vec3 color = vec3(y);

	float pct = plot(st);
	color = (1.0 - pct) * color + pct * vec3(0.0, 1.0, 1.0);

    fragColor = vec4(color, 1.0);
}
