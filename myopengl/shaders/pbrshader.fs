#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform samplerCube skybox;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;


// lights
uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];

uniform vec3 viewPos;

const float PI = 3.14159265359;

uniform bool lightopen;

vec3 getNormalFromNormalMap()//从法线贴图中取法线不对，生成TBN矩阵
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz*2.0 - 1.0;//没明白；了解了

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(T, N));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);

}

vec3 FresnelSchlick(float costheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - costheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;//使用粗糙度的平方会让光照看起来更加自然。
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


void main()
{
   // vec3 albedo = pow((texture(albedoMap, TexCoords).rgb), vec3(2.2));
    vec3 albedo = (texture(albedoMap, TexCoords).rgb);
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao = texture(aoMap, TexCoords).r;
    
    vec3 Lo = vec3(0.0);

    vec3 N = getNormalFromNormalMap();
    //vec3 N   = normalize(Normal);
    vec3 V = normalize(viewPos - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04); //PBR金属流中简单地认为大多数的绝缘体在F0为0.04的时候看起来视觉上是正确的
    F0 = mix(F0, albedo, metallic);

    //4个点光源，只有4个方向的入射光线会影响片段(像素)的着色。
    //可以直接循环N次计算这些入射光线的方向(N也就是场景中光源的数目)。
    for(int i = 0; i < 1; i++)
    {
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V+L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        //cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
        float G = GeometrySmith(N, V, L, roughness);

        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular     = nominator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;// brdf diff+spec

    }

    //vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    //viewdir+normal求r，用 r采样，这里r即是l
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb; 
    //用n与v的角度，配合粗糙度来采样
    vec2 brdf  = texture2D(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).xy;
    vec3 specular = prefilteredColor * (F0 * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular ) * ao;
    // float ratio = 1.00 / 1.52;
    // //vec3 r = refract(-V, N, ratio);
    // vec3 r = reflect(-V, N);
    // vec3 ambient = texture(skybox, r).rgb;

    vec3 color;
    if (lightopen)
         color   = ambient;
    else color =  Lo;
    
    //hdr and gamma     
    color = color / (vec3(1.0) + color);
    color = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(color, 1.0); 
}