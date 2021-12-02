#version 430 core

out vec4 FragColor;

layout (std430, binding = 1) buffer CountBuffer{
    int cnts[];
};

in vec3 modelPos;

uniform vec3 boxMin;
int step = 1;
uniform int resolution;

void main(){
    int res = resolution / 2;
    uint x = int(gl_FragCoord.x);
    uint y = int(gl_FragCoord.y);
    uint z = int(gl_FragCoord.z * res + res);

    uint index = z * (resolution * resolution) + y * resolution + x;
    atomicAdd(cnts[index], 1);
    FragColor = vec4(gl_FragCoord.x/1280, gl_FragCoord.y/720, 0, 1);
}
