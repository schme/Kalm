#version 460
uniform mat4 MVP;
uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 norm;
in vec3 eye;

out vec4 fragColor;

vec2 matcap(vec3 eye, vec3 normal) {
	vec3 r = reflect(eye, normal);
	float m = 2. * sqrt( pow(r.x, 2.) + pow(r.y, 2.) + pow(r.z + 1., 2.));
	return r.xy / m + .5;
}

void main()
{ 
	vec2 uv = matcap(eye, norm).xy;
	fragColor = vec4(texture(texture0, uv).rgb, 1.0);
}
