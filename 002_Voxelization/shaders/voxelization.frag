#version 430 core

out vec4 FragColor;

layout (std430, binding = 1) buffer CountBuffer{
    int cnts[];
};

in vec3 modelPosFrag;

uniform vec3 boxMin;
int step = 1;
uniform int resolution;

void main(){
    int res = resolution / 2;
//    uint x = int(gl_FragCoord.x);
//    uint y = int(gl_FragCoord.y);
//    uint z = int(gl_FragCoord.z * res + res);

    int x = int(modelPosFrag.x * res + res);
    int y = int(modelPosFrag.y * res + res);
    int z = int(modelPosFrag.z * res + res);

    int index = z * (resolution * resolution) + y * resolution + x;

//    int bit = z >> 5;
//    int bitIndex = z & ((1 << 5)-1);
//    atomicOr(cnts[bit], 1<<bitIndex);

    atomicAdd(cnts[index], 1);
    FragColor = vec4(modelPosFrag, 1);
}
