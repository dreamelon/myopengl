#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projection;
uniform mat4 view[3];

uniform mat4 viewProjectionsI[3];

in vec3 worldPosGeom[];
in vec3 modelPosGeom[];

out vec3 worldPosFrag;
out vec3 modelPosFrag;
out vec3 voxelPos;
out vec4 triangleAABB;

int CalculateAxis(){
    vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    vec3 normal = abs(cross(p1, p2));

    if(normal.x >= normal.y && normal.x >= normal.z){
        return 0;
    }
    else if(normal.y >= normal.z){
        return 1;
    }
    else{
        return 2;
    }
}

vec4 CalcAABB(vec4 pos[3]){

    vec4 bbox;
    bbox.xy= max(pos[0].xy, max(pos[1].xy, pos[2].xy));
    bbox.zw= min(pos[0].xy, min(pos[1].xy, pos[2].xy));

    return bbox;
}

//    float A01 = gl_in[1].gl_Position.y - gl_in[0].gl_Position.y;
//    float B01 = gl_in[0].gl_Position.x - gl_in[1].gl_Position.x;
//    float C01 = A01 * gl_in[0].gl_Position.x + B01 * gl_in[0].gl_Position.y;
//
//    C01 = C01 - dot(abs(vec2(A01, B01)), vec2(1, 1));

void main()
{
    int index = CalculateAxis();
    mat4 view = view[index];
    mat4 viewProjectionI = viewProjectionsI[index];

    vec4 pos[3] = vec4[3]
    (
        projection * view * gl_in[0].gl_Position,
        projection * view * gl_in[1].gl_Position,
        projection * view * gl_in[2].gl_Position
    );
    triangleAABB = CalcAABB(pos);

    vec4 trianglePlane;
    trianglePlane.xyz = normalize(cross(pos[1].xyz - pos[0].xyz, pos[2].xyz - pos[0].xyz));
    // the w represent the distance between the triangle and origin
    trianglePlane.w = -dot(pos[0].xyz, trianglePlane.xyz);

    if(trianglePlane.z == 0.0f) 
        return;

    if( dot(trianglePlane.xyz, vec3(0.0, 0.0, 1.0)) < 0.0 ){
        vec4 tmpVert = pos[2];
        pos[2] = pos[1];
        pos[1] = tmpVert;
    }

    vec2 halfPixel = vec2(1.f / 256);
    vec3 plane0 = cross(pos[0].xyw - pos[2].xyw, pos[2].xyw);
    vec3 plane1 = cross(pos[1].xyw - pos[0].xyw, pos[0].xyw);
    vec3 plane2 = cross(pos[2].xyw - pos[1].xyw, pos[1].xyw);
    // (a, b) * x + c = 0;
    // c' = c - V * (a, b)
    // move line along with the vector V
    plane0.z = -dot(halfPixel, abs(plane0.xy));
    plane1.z = -dot(halfPixel, abs(plane1.xy));
    plane2.z = -dot(halfPixel, abs(plane2.xy));

    vec3 intersection0 = cross(plane0, plane1);
    vec3 intersection1 = cross(plane1, plane2);
    vec3 intersection2 = cross(plane2, plane0);
    intersection0 /= intersection0.z;
    intersection1 /= intersection1.z;
    intersection2 /= intersection2.z;

    float z[3];
    z[0] = -(trianglePlane.x * intersection0.x + trianglePlane.y * intersection0.y + trianglePlane.w) / trianglePlane.z;
    z[1] = -(trianglePlane.x * intersection1.x + trianglePlane.y * intersection1.y + trianglePlane.w) / trianglePlane.z;
    z[2] = -(trianglePlane.x * intersection2.x + trianglePlane.y * intersection2.y + trianglePlane.w) / trianglePlane.z;
    pos[0].xyz = vec3(intersection0.xy, z[0]);
    pos[1].xyz = vec3(intersection1.xy, z[1]);
    pos[2].xyz = vec3(intersection2.xy, z[2]);

    for(int i=0; i<3; i++){
        gl_Position = pos[i];
        //gl_Position = projection * view * gl_in[i].gl_Position;
        vec4 voxelPos = viewProjectionI * pos[i];
        voxelPos.xyz /= voxelPos.w;
        worldPosFrag = voxelPos.xyz;
        modelPosFrag = modelPosGeom[i];
        EmitVertex();
    }
    EndPrimitive();

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