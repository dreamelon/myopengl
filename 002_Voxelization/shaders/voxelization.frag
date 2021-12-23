#version 430 core

out vec4 FragColor;

layout (std430, binding = 1) buffer CountBuffer{
    int cnts[];
};

in vec3 modelPosFrag;
in vec3 worldPosFrag;
in vec4 triangleAABB;
in vec2 dilatedPos;

uniform int resolution;

void main(){
    if(dilatedPos.x < triangleAABB.x || dilatedPos.x > triangleAABB.z || 
        dilatedPos.y < triangleAABB.y || dilatedPos.y > triangleAABB.w)
    {
        discard;
    }

    int res = resolution / 2;
//    int x = int(gl_FragCoord.x);
//    int y = int(gl_FragCoord.y);
//    int z = int(gl_FragCoord.z * res + res);

    int x = int(worldPosFrag.x + res);
    int y = int(worldPosFrag.y + res);
    int z = int(worldPosFrag.z + res);

    int index = z * (resolution * resolution) + y * resolution + x;

    int bit = index >> 5;
    int bitIndex = index & ((1 << 5)-1);
    atomicOr(cnts[bit], 1<<bitIndex);

    //atomicAdd(cnts[index], 1);
    FragColor = vec4(modelPosFrag, 1);
}
