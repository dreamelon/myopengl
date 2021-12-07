#version 430 core

out vec4 FragColor;

layout (std430, binding = 1) buffer CountBuffer{
    int cnts[];
};

in vec3 modelPosFrag;
in vec3 worldPosFrag;

uniform int resolution;

void main(){
    int res = resolution / 2;
//    int x = int(gl_FragCoord.x);
//    int y = int(gl_FragCoord.y);
//    int z = int(gl_FragCoord.z * res + res);


    int x = int(modelPosFrag.x * 150 + 150);
    int y = int(modelPosFrag.y * 150 + 150);
    int z = int(modelPosFrag.z * 150 + 150);

    int index = z * (resolution * resolution) + y * resolution + x;

    int bit = index >> 5;
    int bitIndex = index & ((1 << 5)-1);
    atomicOr(cnts[bit], 1<<bitIndex);

    //atomicAdd(cnts[index], 1);
    FragColor = vec4(modelPosFrag, 1);
}
