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
    int res = resolution / 2;
    uint x = int(worldPos.x * res+ res);
    uint y = int(worldPos.y * res+ res);
    uint z = int(worldPos.z * res+ res);

    uint index = z * (resolution * resolution) + y * resolution + x;
    atomicAdd(cnts[index], 1);
    FragColor = vec4(worldPos.z, 0, 0, 1);
}
