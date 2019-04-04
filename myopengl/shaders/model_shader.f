#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D shadowmap;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform sampler2D texture_height1;
uniform sampler2D texture_reflection1;

uniform samplerCube skybox;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    //透视除法，对透视投影
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//变换到屏幕空间(0,1)
	projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0)
	   return 0.0;
	//取深度图中深度值
	//float closestDepth = texture(shadowmap, projCoords.xy).r;
	//进行比较是否在阴影中
	float currentDepth = projCoords.z;
	vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float bias = max(0.05*(1.0 - dot(normal, lightDir)), 0.005);
    //bias = 0.005;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
	for(int x=-1; x <= 1;++x){
	   for(int y=-1; y<=1; ++y){
	       float pcfDepth = texture(shadowmap, projCoords.xy + vec2(x,y)*texelSize).r;
		   shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	   }
	}
	shadow /= 9.0;	
	//float shadow = currentDepth-bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{    

	//vec2 TexCoords = vec2(gl_FragCoord.x/800, gl_FragCoord.y/600);
    vec3 diffusecolor = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 specularcolor = texture(texture_specular1, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);
	//ambient
	vec3 ambient = 0.1 * lightColor;
	//diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor * diffusecolor;
	//specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 64);
	vec3 specular = spec * lightColor * specularcolor;
	
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	//shadow = min(shadow, 0.75);
	vec3 lighting = ambient + (1.0-shadow) * (diffuse);
    
	//FragColor = vec4(lighting, 1.0);
	float reflectintensity = texture(texture_reflection1, fs_in.TexCoords).r;
	vec4 reflectColor = vec4(0.0, 0.0, 0.0, 0.0);
	if(reflectintensity > 0.1){
		vec3 I = normalize(fs_in.FragPos - viewPos);
	    vec3 R = reflect(I, normalize(fs_in.Normal));
	    reflectColor = reflectintensity * texture(skybox, R);
	}
    FragColor = vec4(lighting, 1.0)
	           + reflectColor;
}