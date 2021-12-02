#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 modelPos;

void main()
{
    modelPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}