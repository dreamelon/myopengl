#version 430 core

out vec4 FragColor;

layout (std430, binding = 1) buffer CountBuffer{
    int cnts[];
};

in vec3 worldPos;

uniform vec3 boxMin;
int step = 1;
uniform int resolution;

void main(){

    uint x = int(worldPos.x * resolution + resolution / 2);
    uint y = int(worldPos.y * resolution + resolution / 2);
    uint z = int(worldPos.z * resolution + resolution / 2);

    uint index = z * (resolution * resolution) + y * resolution + x;
    atomicAdd(cnts[index], 1);
    FragColor = vec4(worldPos.z, 0, 0, 1);
}
