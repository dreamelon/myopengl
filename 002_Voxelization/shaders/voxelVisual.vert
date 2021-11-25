#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;
layout (location = 3) in vec3 aOffset;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 worldPos;

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    gl_Position =  projection * view * vec4(worldPos, 1.0);
}