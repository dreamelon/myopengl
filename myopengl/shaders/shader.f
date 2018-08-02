#version 330 core
out vec4 FragColor; 

in vec3 Normal; 
in vec3 FragPos;

in vec2 TexCoords;

uniform vec3 viewPos;

struct Material{
	//材质本身环境光颜色几乎等于漫反射颜色，故舍去
    sampler2D diffuse;	
	sampler2D specular;
	//sampler2D emission;
	float shininess;
};

struct SpotLight{
	vec3 position;
    vec3 direction;
	float cutOff;
	float outercutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLight;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	//定向
	vec3 result = CalDirLight(dirLight, norm, viewDir);
	//点光源
	for(int i=0; i<NR_POINT_LIGHTS; i++)
		result += CalPointLight(pointLights[i], norm, FragPos, viewDir); 
	//聚光
	result += CalSpotLight(spotLight, norm, FragPos, viewDir);
	
  
    FragColor = vec4(result, 1.0);

}

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	//镜面光
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	//合并
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	return ambient+diffuse+specular;
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	//镜面光
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	
	//合并
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	//镜面光
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outercutOff;
	float intensity = clamp((theta - light.outercutOff)/epsilon, 0.0, 1.0);

	
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

  return ambient + diffuse + specular;	
}