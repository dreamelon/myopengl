#version 430 core

out vec4 FragColor;

layout (std430, binding = 0) buffer CountBuffer{
    int cnts[];
};

in vec3 worldPos;

uniform vec3 boxMin;
int step = 1;
uniform vec3 resolution;

void main(){

    uint x = uint((worldPos.x - boxMin.x) / step);
    uint y = uint((worldPos.y - boxMin.y) / step);
    uint z = uint((worldPos.z - boxMin.z) / step);

    uint index = uint(z * (resolution.y * resolution.x) + y * resolution.x + x);
    atomicAdd(cnts[index], 1);
    FragColor = vec4(0.1, 0.4, 0.2, 1);
}
