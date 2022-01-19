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

// Noise by iq
float hash( float n ) { return fract(sin(n)*753.5453123); }
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}

float fbm(vec3 x)
{
    float ret = noise(x);
    ret += noise(x * 2.0) / 2.0;
    ret += noise(x * 4.0) / 4.0;
    ret += noise(x * 8.0) / 8.0;
    ret += noise(x * 16.0) / 16.0;
    return ret;
}

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ) {
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}


vec3 sphereTrace(vec3 rayOrigin, vec3 rayDirection)
{
    return vec3();
}

void main()
{
    float aspect = resolution.x/resolution.y;
    float fov = 90;

    vec2 uv = (gl_FragCoord.xy + vec2(0.5))/resolution/4.0;
    uv.x = (uv.x * 2.0 - 1.0) * aspect;
    uv.y = 1.0 - uv.y * 2.0; // flip y as well
    uv = uv * tan(radians(fov) / 2);

    vec3 p = vec3(gl_FragCoord.x - resolution.x / 2,
            resolution.y / 2 - gl_FragCoord.y,
            -(resolution.y/2) / tan(radians(fov) * 0.5));

    p = normalize(p);

    vec3 rayOrigin = vec3(0., 0., 0.);
    vec3 rayDirection = vec3(uv, -1.) - rayOrigin;
    rayDirection = normalize(rayDirection);

    vec3 outputColor = vec3(0.0);
    outputColor = sphereTrace();

    vec3 gammaCorrectedColor = pow(outputColor, vec3(1.0 / 2.2));
    fragColor = vec4(gammaCorrectedColor, 1.0);
}
