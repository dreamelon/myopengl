#version 330 core
out vec4 FragColor; 

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
	mat3 TBN;
}fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{	 
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(fs_in.TBN * normal);
	
	vec3 lightColor = vec3(0.3);
	vec3 objectColor = texture(diffuseMap, fs_in.TexCoords).rgb;
	//ambient
	vec3 ambient = 0.1 * objectColor;
	//diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * objectColor;
	//specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	
	vec3 result = ambient + diffuse + specular;
	//FragColor = vec4(1.0);
    FragColor = vec4(result, 1.0);
}