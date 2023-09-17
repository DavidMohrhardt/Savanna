// A physically based material model for the simulation of the interaction of light with surfaces.
//
// The material is defined by its reflectance function, which describes how light is reflected at the surface.
// Material inputs are Albedo, Metallic, Roughness, Ambient Occlusion, Normal, Height, and Emission.
//
// The material is based on the Cook-Torrance model, which is a microfacet model for physically based rendering.
// Cook-Torrance is defined as follows:
// f(l, v) = (F(l, h) * G(l, v, h) * D(h)) / (4 * (n.l) * (n.v))
// where:
// f(l, v) is the BRDF
// l is the light vector
// v is the view vector
// h is the half vector
// F(l, h) is the Fresnel term
// G(l, v, h) is the geometric term
// D(h) is the normal distribution function
// n is the surface normal
//
// The Fresnel term is defined as follows:
// F(l, h) = F0 + (1 - F0) * (1 - (l.h))^5
// where:
// F0 is the reflectance at normal incidence
// l is the light vector
// l.h is the half vector
//
// The geometric term is defined as follows:
// G(l, v, h) = min(1, (2 * (n.h) * (n.v)) / (v.h), (2 * (n.h) * (n.l)) / (v.h))
//
// The normal distribution function is defined as follows:
// D(h) = (n.h)^2 * (a^2 - 1) / (pi * a^2 * (n.h)^2 + (a^2 - 1))

// Bindings:
layout (binding = 0) uniform sampler2D u_AlbedoMap;
layout (binding = 1) uniform sampler2D u_MetallicMap;
layout (binding = 2) uniform sampler2D u_RoughnessMap;
layout (binding = 3) uniform sampler2D u_AOMap;
layout (binding = 4) uniform sampler2D u_NormalMap;
layout (binding = 5) uniform sampler2D u_HeightMap;
layout (binding = 6) uniform sampler2D u_EmissionMap;

// Inputs:
layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_Position;

// Material parameters:
layout (std140) uniform Material
{
    vec4 u_Albedo;
    float u_Metallic;
    float u_Roughness;
    float u_AO;
    float u_HeightScale;
    float u_Emission;
};

// Outputs:
layout (location = 0) out vec4 o_Color;
layout (location = 1) out vec3 o_Normal;
layout (location = 2) out vec3 o_Position;
layout (location = 3) out vec3 o_Emission;

// Fragment shader:
mat3 ComputeTangentSpace(vec3 normal, vec3 position, vec2 texCoord)
{
    // Calculate the tangent space basis vectors for the fragment
    vec3 q1 = dFdx(position);
    vec3 q2 = dFdy(position);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);
    vec3 N = normal;
    vec3 T = normalize(q1 * st2.t - q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    return mat3(T, B, N);
}

void main()
{
    vec3 view = normalize(-v_Position);
    vec3 light = normalize(vec3(0.0, 1.0, 0.0));
    vec3 halfVector = normalize(light + view);
    float fresnel = 0.04 + (1.0 - 0.04) * pow(1.0 - dot(light, halfVector), 5.0);

    // Naively read all the textures
    vec4 albedo = texture(u_AlbedoMap, v_TexCoord);
    float metallic = texture(u_MetallicMap, v_TexCoord).r;
    float roughness = texture(u_RoughnessMap, v_TexCoord).r;
    float ao = texture(u_AOMap, v_TexCoord).r;
    vec3 normal = normalize(texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0);
    float height = texture(u_HeightMap, v_TexCoord).r * u_HeightScale;
    vec3 emission = texture(u_EmissionMap, v_TexCoord).rgb * u_Emission;

    vec4 color = vec4(0.0);

    // vec3 normal = normalize(v_Normal);

    // float height = texture(u_HeightMap, v_TexCoord).r * u_HeightScale;
    // vec3 emission = texture(u_EmissionMap, v_TexCoord).rgb * u_Emission;

    // // Calculate the tangent space basis vectors for the fragment
    // ComputeTangentSpace(normal, v_Position, v_TexCoord);

    // // Calculate the view vector
    // vec3 view = normalize(-v_Position);

    // // Calculate the reflection vector
    // vec3 reflectionVector = reflect(-view, normal);

    // // Calculate the diffuse irradiance
    // vec4 albedo = texture(u_AlbedoMap, v_TexCoord) * u_Albedo;
    // vec3 diffuse = texture(u_IrradianceMap, normal).rgb * albedo.rgb;

    // float metallic = texture(u_MetallicMap, v_TexCoord).r * u_Metallic;
    // float roughness = texture(u_RoughnessMap, v_TexCoord).r * u_Roughness;
    // float ao = texture(u_AOMap, v_TexCoord).r * u_AO;

    // // Calculate the specular reflection
    // vec3 specular = texture(u_PrefilterMap, reflectionVector, roughness * roughness).rgb;

    // // Calculate the ambient lighting
    // vec3 ambient = (diffuse + specular) * ao;

    // // Calculate the final color
    // vec3 color = ambient + emission;

    // // Write the final color
    // o_Color = vec4(color, albedo.a);

    // // Write the normal
    // o_Normal = TBN * normal;

    // // Write the position
    // o_Position = v_Position + normal * height;

    // // Write the emission
    // o_Emission = emission;
}
