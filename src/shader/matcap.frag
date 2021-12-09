R"(
#version 460
uniform mat4 MVP;
uniform float time;
uniform vec3 eye;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 pos;
in vec4 color;
in vec2 tex0;
in vec3 norm;

out vec4 fragColor;

vec2 matcap(vec3 eye, vec3 normal) {
  vec3 reflected = reflect(eye, normal);
  float m = 2.8284271247461903 * sqrt( reflected.z+1.0 );
  return reflected.xy / m + 0.5;
}

void main()
{ 
	vec2 uv = matcap(pos - eye, norm).xy;
  fragColor = texture(texture0, uv);
}

)";
