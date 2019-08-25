<<<<<<< HEAD
 #version 330 core
=======
#version 330 core
>>>>>>> f4fdc2381dc4ac8f19c88df4c1927d10c6a39a8d
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
<<<<<<< HEAD
{    
=======
{   
	//vec2 TexCoords = vec2(gl_FragCoord.x/800, gl_FragCoord.y/600);
>>>>>>> f4fdc2381dc4ac8f19c88df4c1927d10c6a39a8d
    vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

	vec3 lightColor = vec3(0.1);
	vec3 diffuseColor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	//ambient
	vec3 ambient = 0.1 * diffuseColor;
	//diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
<<<<<<< HEAD
	vec3 diffuse = diff * diffuseColor;
=======
	vec3 diffuse = diff * diffuseColor * 0.5;
>>>>>>> f4fdc2381dc4ac8f19c88df4c1927d10c6a39a8d
	//specular
    //vec3 specularColor = texture(texture_specular1, fs_in.TexCoords).rgb;
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);
	vec3 specular = spec * lightColor;
	
	vec3 result = ambient + diffuse + specular;
	//FragColor = vec4(1.0);
    FragColor = vec4(result, 1.0);
    //FragColor = 0.05 * texture(texture_diffuse1, fs_in.TexCoords)  + 0.2 * texture(texture_specular1, fs_in.TexCoords);
}