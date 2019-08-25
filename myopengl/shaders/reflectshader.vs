#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

<<<<<<< HEAD:myopengl/shaders/reflectshader.vs
out vec3 Normal;
out vec3 Position;
=======
out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} vs_out;
>>>>>>> f4fdc2381dc4ac8f19c88df4c1927d10c6a39a8d:myopengl/shaders/model_shader.v

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
<<<<<<< HEAD:myopengl/shaders/reflectshader.vs
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
=======
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	//vs_out.TexCoords = vec2(gl_Position.x / gl_Position.w, gl_Position.y / gl_Position.w) * 0.5 + 0.5;;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}  
>>>>>>> f4fdc2381dc4ac8f19c88df4c1927d10c6a39a8d:myopengl/shaders/model_shader.v
