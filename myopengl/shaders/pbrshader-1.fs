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

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform samplerCube cubeMap;

// lights
uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];

uniform vec3 viewPos;

const float PI = 3.14159265359;

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

// efficient VanDerCorpus calculation. 基于vdc产生hammersley sequence
float RadicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

//ggx importance sampling NDF结合GGx 得到h向量
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.0*PI*Xi.x;
    float costheta = sqrt((1.0-Xi.y) / (1.0+(a*a-1.0)*Xi.y));//存疑。。
    float sintheta = sqrt(1.0 - costheta*costheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sintheta;
    H.y = sin(phi) * sintheta;
    H.z = costheta;

    vec3 up = abs(N.z)<0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    //vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 samplevec = tangent * H.x + bitangent * H.y + N * H.z;

    return normalize(samplevec);
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
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);//这里使用h而不是n，
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

    uint sampler = 512u;
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 n = N;
    //vec3 n = vec3(0.0, 0.0, 1.0);//切空间
    vec3 v = normalize(viewPos - WorldPos);
    for (uint i = 0u; i < sampler; i++) {
        vec2 xi = Hammersley(i, sampler);
        vec3 h = ImportanceSampleGGX(xi, n, roughness);
        vec3 l = 2.0 * dot(v, h) * h - v;
        float ndotv = max(0.0, dot(n, v));
        float ndoth = max(0.0, dot(n, h));
        float vdoth = max(0.0, dot(v, h));
        float ndotl = max(0.0, dot(n, l));
        if (ndotl > 0.0) {
            vec3 light = textureLod(cubeMap, l, roughness).xyz;
            float G = GeometrySmith(n, v, l, roughness);
            vec3 F = FresnelSchlick(max(dot(h, v), 0.0), F0);
            specular = specular + light * F * G * vdoth / (ndoth * ndotv);
        }
    }
    specular = specular / sampler;

    vec3 ambient = (kD * diffuse + specular ) * ao;

    
    vec3 color   = ambient ;  
    
    //hdr and gamma     
    color = color / (vec3(1.0) + color);
    color = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(color, 1.0); 
}