#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
	mat3 TBN;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	//vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	// re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(T, N);
	vs_out.TBN = mat3(T,B,N);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.TexCoords = aTexCoords;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
}