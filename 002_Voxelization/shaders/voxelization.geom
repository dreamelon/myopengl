#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projection;
uniform mat4 view[3];

in vec3 modelPosGeom[];

out vec3 modelPosFrag;

void main()
{
    vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    vec3 normal = normalize(cross(p1, p2));

    if(normal.x >= normal.y && normal.x >= normal.z){
        for(int i=0; i<3; i++){
            gl_Position = vec4(gl_in[i].gl_Position.yz, 0, 1);
            modelPosFrag = modelPosGeom[i];
            EmitVertex();
        }
        EndPrimitive();
    }
    else if(normal.y >= normal.z){
        for(int i=0; i<3; i++){
            gl_Position = vec4(gl_in[i].gl_Position.xz, 0, 1);
            modelPosFrag = modelPosGeom[i];
            EmitVertex();
        }
        EndPrimitive();
    }
    else{
        for(int i=0; i<3; i++){
            gl_Position = vec4(gl_in[i].gl_Position.xy, 0, 1);
            modelPosFrag = modelPosGeom[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}