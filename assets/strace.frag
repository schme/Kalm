#version 460

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float time;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 cameraDir;
uniform float cameraFov;
uniform float cameraYaw;
uniform float cameraPitch;
uniform float cameraRoll;

in vec2 tex0;
in vec3 pos;
in vec3 norm;
out vec4 fragColor;

#define PI 3.14159265359

#define saturate(x) (clamp((x), 0.0, 1.0))

vec3 rotateX(vec3 x, float ang)
{
    float c = cos(ang);
    float s = sin(ang);
    return vec3(x.x, x.y * c - x.z * s, x.z * c + x.y * s);
}

vec3 rotateY(vec3 x, float ang)
{
    float c = cos(ang);
    float s = sin(ang);
    return vec3(x.x * c - x.z * s, x.y, x.z * c + x.x * s);
}

vec3 rotateZ(vec3 x, float ang)
{
    float c = cos(ang);
    float s = sin(ang);
    return vec3(x.x * c - x.y * s, x.y * c + x.x * s, x.z);
}

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

struct Light {
    vec3 pos;
    vec3 color;
    float intensity;
};

struct Material {
    vec3 albedo;
    float metalness;
    float roughness;
    vec3 emissive;
};

struct SceneResult {
    float d;
    Material mat;
};

Light[2] lights = Light[](
        Light(vec3(2.0, 1.0, 5.0), vec3(1.0), 3.0),
        Light(vec3(-3.0, 2.0, -3.0), hsb2rgb(vec3(0.14, 1.0, 1.0)), 9.5));

Material dummyMaterial() {
    return Material(vec3(0.3), 0.0, 0.5, vec3(0.0));
}

Material nullMaterial() {
    return Material(vec3(0), 0.0, 0.0, vec3(0.0));
}

float plane(in vec3 from, in vec3 point, in vec3 norm)
{
    return dot(norm, from - point);
}

float sphere(in vec3 from, in vec3 center, in float rad)
{
    return length(from - center) - rad;
}

float scene(in vec3 from, float maxDistance)
{
    float d = min( maxDistance + 1.0, min( //min(
        min(sphere(from, vec3(0.2, 0.0, -5.), 1.0),
            sphere(from, vec3(-2.0, 1.0, -7.), 2.0)),
        min(plane(from, vec3(0.0, 10.0, 0.0), vec3(0.0, -1.0, 0.0)),
            plane(from, vec3(0.0, -1.0, 0.0), vec3(0.0, 1.0, 0.0))))/*,
        min(sphere(from, lights[0].pos, 0.1),
            sphere(from, lights[1].pos, 0.1)))*/);

    return d;
}

float traceShadow(vec3 rayOrigin, vec3 rayDirection, float maxDistance)
{
    const float eps = 10e-5;
    float t = 0;

    while (t < maxDistance) {
        float minDistance = maxDistance + 1;
        vec3 from = rayOrigin + t * rayDirection;

        float d = scene(from, maxDistance);
        if (d <= eps * t) {
            return 1.0;
        }
        t += d;
    }

    return 0.0f;
}

/**
 * This section is code from:
 * https://learnopengl.com/PBR/Theory
 * By: Joey de Vries
 * License: CC BY-NC 4.0 (https://creativecommons.org/licenses/by-nc/4.0/)
 */


// Trowbridge-Reitz GGX
float DistributionGGX(in vec3 N, in vec3 H, in float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(in float NdotV, in float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float GeometrySmith(in vec3 N, in vec3 V, in vec3 L, in float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(in float cosTheta, in vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 getF0(in Material mat)
{
    vec3 F0 = vec3(0.04);
    F0      = mix(F0, mat.albedo.rgb, mat.metalness);
    return F0;
}

/** 
 * End of section
 */

vec3 kDiff(in Material mat)
{
    return mat.albedo / PI;
}

// Cook-Torrance specular
vec3 kSpec(in vec3 p, in vec3 N, in vec3 V, in vec3 L, in vec3 H, in Material mat)
{
    float cosTheta = dot(L, N);

    float kDirect = pow(mat.roughness + 1, 2) / 8;
    /*float kIBL = pow(mat.roughness, 2) * 0.5;*/

    float D = DistributionGGX( N, H, mat.roughness);
    float G = GeometrySmith( N, V, L, kDirect);
    vec3 F = fresnelSchlick( cosTheta, getF0(mat));

    vec3 nom = D * G * F;
    vec3 denom = vec3(4 * dot(V, N) * dot(V, N));

    return nom / denom;
}

vec3 shade(in vec3 p, in vec3 eyeDir, in vec3 N)
{
    Material mat = Material(vec3(1.0), 0.0, 0.5, vec3(0.0));

    vec3 R = mat.emissive;

    // Cook-Torrance BRDF

    for (int i=0; i < lights.length(); ++i) {

        vec3 L = normalize(lights[i].pos - p);
        vec3 V = -eyeDir;
        vec3 H = normalize(V + L);

        vec3 diff = kDiff(mat);
        vec3 spec = kSpec(p, N, V, L, H, mat);

        vec3 fR = diff + spec;
        
        R += diff + spec;
    }


    return R;
}


vec3 trace(in vec3 rayOrigin, in vec3 rayDirection)
{
    const float maxDistance = 100;
    const float eps = 10e-6;

    float t = 0;

    while (t < maxDistance) {
        vec3 from = rayOrigin + t * rayDirection;
        float d = scene(from, maxDistance);

        if (d <= eps * t) {
            break;
        }
        t += d;
    }

    if (t < maxDistance) {
        vec3 from = rayOrigin + t * rayDirection;
        float delta = 10e-5;
        vec3 n = vec3(
                scene(from + vec3(delta, 0, 0), maxDistance) - scene(from + vec3(-delta, 0, 0), maxDistance),
                scene(from + vec3(0, delta, 0), maxDistance) - scene(from + vec3(0, -delta, 0), maxDistance),
                scene(from + vec3(0, 0, delta), maxDistance) - scene(from + vec3(0, 0, -delta), maxDistance)
            );
        n = normalize(n);

        vec3 col = shade(from, rayDirection, n);
        return col;
    }

    return vec3(0);
}


void main()
{
    vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / resolution.y;
    float fov = cameraFov;
    float fovTan = tan(radians(fov) / 2.0);

    vec3 rayDirection = normalize(vec3(uv * fovTan, -1.0));

    rayDirection = rotateX(rayDirection, radians(cameraPitch));
    rayDirection = rotateY(rayDirection, radians(cameraYaw));
    rayDirection = rotateZ(rayDirection, radians(cameraRoll));

    vec3 rayOrigin = cameraPos;

    vec3 color = trace(rayOrigin, rayDirection);

    vec3 gammaCorrectedColor = pow(color, vec3(1.0 / 2.2));
    fragColor = vec4(gammaCorrectedColor, 1.0);
}
