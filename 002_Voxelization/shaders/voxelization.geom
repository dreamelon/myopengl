#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projection;
uniform mat4 viewX;
uniform mat4 viewY;
uniform mat4 viewZ;

in vec3 worldPosGeom[];
in vec3 modelPosGeom[];

out vec3 worldPosFrag;
out vec3 modelPosFrag;
out vec3 voxelPos;

vec4 calcAABB(vec4 pos[3]){

    vec4 bbox;
    bbox.xy= max(pos[0].xy, max(pos[1].xy, pos[2].xy));
    bbox.zw= min(pos[0].xy, min(pos[1].xy, pos[2].xy));

    return bbox;
}

void main()
{
    vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    vec3 normal = abs(cross(p1, p2));

    vec4 pos[3] = vec4[3]
    (
        projection * viewX * gl_in[0].gl_Position,
        projection * viewX * gl_in[1].gl_Position,
        projection * viewX * gl_in[2].gl_Position
    );

    float A01 = gl_in[1].gl_Position.y - gl_in[0].gl_Position.y;
    float B01 = gl_in[0].gl_Position.x - gl_in[1].gl_Position.x;
    float C01 = A01 * gl_in[0].gl_Position.x + B01 * gl_in[0].gl_Position.y;

    C01 = C01 - dot(abs(vec2(A01, B01)), vec2(1, 1));

    if(normal.x >= normal.y && normal.x >= normal.z){
        for(int i=0; i<3; i++){
            gl_Position = projection * viewX * gl_in[i].gl_Position;
            //gl_Position = vec4(gl_in[i].gl_Position.xy, 0, 1);
            worldPosFrag = worldPosGeom[i];
            modelPosFrag = modelPosGeom[i];
            EmitVertex();
        }
        EndPrimitive();
    }
    else if(normal.y >= normal.z){
        for(int i=0; i<3; i++){
            gl_Position = projection * viewY * gl_in[i].gl_Position;
            //gl_Position = vec4(gl_in[i].gl_Position.xy, 0, 1);
            worldPosFrag = worldPosGeom[i];
            modelPosFrag = modelPosGeom[i];
            EmitVertex();
        }
        EndPrimitive();
    }
    else{
        for(int i=0; i<3; i++){
            gl_Position = projection * viewZ * gl_in[i].gl_Position;
            //gl_Position = vec4(gl_in[i].gl_Position.xy, 0, 1);
            worldPosFrag = worldPosGeom[i];
            modelPosFrag = modelPosGeom[i];
            EmitVertex();
        }
        EndPrimitive();
    }

//    if(normal.x >= normal.y && normal.x >= normal.z){
//        for(int i=0; i<3; i++){
//            gl_Position = vec4(gl_in[i].gl_Position.yz, 0, 1);
//            modelPosFrag = modelPosGeom[i];
//            EmitVertex();
//        }
//        EndPrimitive();
//    }
//    else if(normal.y >= normal.z){
//        for(int i=0; i<3; i++){
//            gl_Position = vec4(gl_in[i].gl_Position.xz, 0, 1);
//            modelPosFrag = modelPosGeom[i];
//            EmitVertex();
//        }
//        EndPrimitive();
//    }
//    else{
//        for(int i=0; i<3; i++){
//            gl_Position = vec4(gl_in[i].gl_Position.xy, 0, 1);
//            modelPosFrag = modelPosGeom[i];
//            EmitVertex();
//        }
//        EndPrimitive();
//    }
}