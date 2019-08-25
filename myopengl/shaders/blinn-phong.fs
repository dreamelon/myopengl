#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform sampler2D normalMap;

uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];
uniform vec3 viewPos;

vec3 getNormalFromNormalMap(vec3 WorldPos, vec3 Normal, vec2 TexCoords)//从法线贴图中取法线不对，生成TBN矩阵
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

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse 
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / distance * distance;
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    
    vec3 normal = getNormalFromNormalMap(fs_in.FragPos, fs_in.Normal,  fs_in.TexCoords);
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 1; ++i)
        lighting += BlinnPhong(normal, fs_in.FragPos, lightPositions[i], lightColors[i] * 0.005);
    color *= lighting;
  
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0);
}