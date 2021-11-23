#version 330 core

out vec4 FragColor;

layout (std430, binding = 0) buffer CountBuffer{
    int cnts[];
};

void main(){

    FragColor = vec4(0.1, 0.4, 0.2, 1);
}
