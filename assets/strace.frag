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

const int numLights = 3;
Light[numLights] lights = Light[](
        Light(vec3(1.0, sin(time) * 0.5 + 0.2, -3.0), vec3(1.9, 1.32, 0.32)),
        Light(vec3(-4.0, sin(time * 0.5) * 0.5, cos(time * 0.5) * 0.5 + -5.0), vec3(1.4, 0.92, 0.58)),
        Light(vec3(-3.0, 9.0, -4.0), vec3(22, 22, 21))
);

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

float box(in vec3 from, in vec3 pos, in vec3 bounds)
{
    vec3 q = abs(from - pos) - bounds;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float roundedBox(in vec3 from, in vec3 pos, in vec3 bounds, float rad)
{
    return box(from, pos, bounds) - rad;
}

Material getMaterial(int indx)
{
    // 0 - room
    // 1 - blue
    // 2 - gold
    // 3 - ground
    // 4 - atmoshpere
    const Material[5] materials = Material[](
        Material(vec3(0.35), 1.0, 0.90, vec3(0.0)),
        Material(vec3(0.0, 0.38, 0.78), 0.0, 0.72, vec3(0.0)),
        Material(vec3(1.0, 0.782, 0.344), 1.0, 0.37, vec3(0.0)),
        Material(vec3(0.26, 0.75, 0.344), 0.0, 0.92, vec3(0)),
        Material(vec3(1), 0.0, 0.1, hsb2rgb(vec3(0.29, 1.9, 1.0)))
    );

    return materials[indx];
}

Material mixMaterial(Material a, Material b, float t)
{
    t = saturate(t);
    return Material(
        mix(a.albedo, b.albedo, t),
        mix(a.metalness, b.metalness, t),
        mix(a.roughness, b.roughness, t),
        mix(a.emissive, b.emissive, t));
}

SceneResult scene(in vec3 from, in float maxDistance)
{
    float dSphere1 = min( maxDistance + 1.0, sphere(from, vec3(0.2, 0.0, -5.), 1.0));

    float dSphere2 = min( maxDistance + 1.0, sphere(from, vec3(-2.0, 1.0, -7.), 1.5));

    float dPlane = min( maxDistance + 1.0, 
        min(min(plane(from, vec3(0.0, 10.0, 0.0), vec3(0.0, -1.0, 0.0)),
            plane(from, vec3(0.0, -1.0, 0.0), vec3(0.0, 1.0, 0.0))),
            plane(from, vec3(0.0, 0.0, -20.0), vec3(0.0, 0.0, 1.0))));

    vec3 barSize = vec3(1.0, 0.17, 0.28);

    float dBox = roundedBox(from, vec3(-2.0, -1.0 + barSize.y + 0.02, -4.0), barSize, 0.04);

    float dLight1 = min(maxDistance + 1.0, sphere(from, lights[0].pos, 0.2));
    float dLight2 = min(maxDistance + 1.0, sphere(from, lights[1].pos, 0.2));
    float dLight3 = min(maxDistance + 1.0, sphere(from, lights[2].pos, 0.2));

    float d = min(min(min(dSphere1, dSphere2), min(dPlane, dBox)), min(dLight1, min(dLight2, dLight3)));

    if (d == dPlane)
        return SceneResult(d, getMaterial(0));

    if (d == dSphere1)
        return SceneResult(d, getMaterial(1));

    if (d == dSphere2 || d == dBox)
        return SceneResult(d, getMaterial(2));

    Light light = Light(vec3(0), vec3(0));
    if (d == dLight1)
         light = lights[0];
    else if (d == dLight2)
        light = lights[1];
    else if (d == dLight3)
        light = lights[2];
    else
        return SceneResult(d, nullMaterial());

    Material mat = Material(vec3(1.0), 0.0, 1.0, light.color);
    return SceneResult(d, mat);
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
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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

// Cook-Torrance specular
vec3 brdf(in vec3 p, in vec3 N, in vec3 V, in vec3 Li, in float cosTheta, in Material mat)
{
    vec3 H = normalize(V + Li);

    float roughness = mat.roughness;

    float kDirect = (roughness * roughness);
    float kIBL = pow(roughness, 2) * 0.5;

    float k = kDirect;

    float D = DistributionGGX( N, H, k);
    float G = GeometrySmith( N, V, Li, k);
    vec3 F = fresnelSchlick( cosTheta, getF0(mat));

    vec3 nom = D * G * F;
    float denom = 4.0 * max(dot(V, N), 0.0) * max(dot(V, N), 0.0);

    return nom / denom;
}

float lightAttenuation(in vec3 p, in vec3 lightPos)
{
    float d = distance(p, lightPos);
    return 1.0 / (d * d);
}

vec3 shade(in vec3 p, in vec3 eyeDir, in vec3 N, in Material mat)
{
    vec3 Lo = mat.emissive;

    // Cook-Torrance BRDF
    for (int i=0; i < lights.length(); ++i) {
        vec3 Li = normalize(lights[i].pos - p);
        vec3 V = normalize(eyeDir);
        float cosTheta = max(dot(N, Li), 0.0);

        vec3 kS = brdf(p, N, V, Li, cosTheta, mat);
        vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - mat.metalness;

        float attenuation = lightAttenuation(p, lights[i].pos);
        vec3 radiance = lights[i].color * attenuation;

        Lo += (kD * mat.albedo / PI + kS) * radiance * cosTheta;
    }

    return Lo;
}


vec3 trace(in vec3 rayOrigin, in vec3 rayDirection)
{
    const float maxDistance = 200;
    const float eps = 10e-5;

    float t = 0;

    SceneResult res = SceneResult(maxDistance + 1, nullMaterial());

    while (t < maxDistance) {
        vec3 from = rayOrigin + t * rayDirection;
        res = scene(from, maxDistance);

        if (res.d <= eps * t) {
            break;
        }
        t += res.d;
    }

    if (t < maxDistance) {
        vec3 from = rayOrigin + t * rayDirection;
        float delta = 10e-5;
        vec3 n = normalize(vec3(
                scene(from + vec3(delta, 0, 0), maxDistance).d - scene(from + vec3(-delta, 0, 0), maxDistance).d,
                scene(from + vec3(0, delta, 0), maxDistance).d - scene(from + vec3(0, -delta, 0), maxDistance).d,
                scene(from + vec3(0, 0, delta), maxDistance).d - scene(from + vec3(0, 0, -delta), maxDistance).d
            ));

        vec3 col = shade(from, -rayDirection, n, res.mat);
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

	// Reinhard
	vec3 tonemappedColor = color / (color + vec3(1.0));
    vec3 gammaCorrectedColor = pow(tonemappedColor, vec3(1.0 / 2.2));
	fragColor = vec4(gammaCorrectedColor, 1.0);
}
