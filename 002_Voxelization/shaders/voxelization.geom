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
out vec2 dilatedPos;
out vec4 triangleAABB;

vec2 halfPixel = vec2(1.f / 256);

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
    bbox.xy= min(pos[0].xy, min(pos[1].xy, pos[2].xy));
    bbox.zw= max(pos[0].xy, max(pos[1].xy, pos[2].xy));

    bbox.xy -= halfPixel;
    bbox.zw += halfPixel;

    return bbox;
}

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

    float A01 = pos[1].y - pos[0].y;
    float B01 = pos[0].x - pos[1].x;
    float C01 = pos[1].x * pos[0].y - pos[0].x * pos[1].y;

    float A12 = pos[2].y - pos[1].y;
    float B12 = pos[1].x - pos[2].x;
    float C12 = pos[2].x * pos[1].y - pos[1].x * pos[2].y;

    float A20 = pos[0].y - pos[2].y;
    float B20 = pos[2].x - pos[0].x;
    float C20 = pos[0].x * pos[2].y - pos[2].x * pos[0].y;

    C01 -= dot(abs(vec2(A01, B01)), halfPixel);
    C12 -= dot(abs(vec2(A12, B12)), halfPixel);
    C20 -= dot(abs(vec2(A20, B20)), halfPixel);

//    vec2 intersection[3];
//    intersection[0].y = (C01 * A20 - C20 * A01) / (A01 * B20 - A20 * B01);
//    intersection[0].x = (C20 * B01 - C01 * B20) / (A01 * B20 - A20 * B01);
//
//    intersection[1].y = (C12 * A01 - C01 * A12) / (A12 * B01 - A01 * B12);
//    intersection[1].x = (C20 * B01 - C01 * B20) / (A12 * B01 - A01 * B12);
//
//    intersection[2].y = (C20 * A12 - C12 * A20) / (A20 * B12 - A12 * B20);
//    intersection[2].x = (C12 * B20 - C20 * B12) / (A20 * B12 - A12 * B20);
//
//    pos[0].xy = intersection[0];
//    pos[1].xy = intersection[1];
//    pos[2].xy = intersection[2];

    vec3 plane0 = cross(pos[0].xyw - pos[2].xyw, pos[2].xyw);
    vec3 plane1 = cross(pos[1].xyw - pos[0].xyw, pos[0].xyw);
    vec3 plane2 = cross(pos[2].xyw - pos[1].xyw, pos[1].xyw);
    // (a, b) * x + c = 0;
    // c' = c - V * (a, b)
    // move line along with the vector V
    plane0.z -= dot(halfPixel, abs(plane0.xy));
    plane1.z -= dot(halfPixel, abs(plane1.xy));
    plane2.z -= dot(halfPixel, abs(plane2.xy));

    vec3 intersection[3];
    intersection[0] = cross(plane0, plane1);
    intersection[1] = cross(plane1, plane2);
    intersection[2] = cross(plane2, plane0);
    intersection[0] /= intersection[0].z;
    intersection[1] /= intersection[1].z;
    intersection[2] /= intersection[2].z;

    float z[3];
    z[0] = -(trianglePlane.x * intersection[0].x + trianglePlane.y * intersection[0].y + trianglePlane.w) / trianglePlane.z;
    z[1] = -(trianglePlane.x * intersection[1].x + trianglePlane.y * intersection[1].y + trianglePlane.w) / trianglePlane.z;
    z[2] = -(trianglePlane.x * intersection[2].x + trianglePlane.y * intersection[2].y + trianglePlane.w) / trianglePlane.z;
    pos[0].xyz = vec3(intersection[0].xy, z[0]);
    pos[1].xyz = vec3(intersection[1].xy, z[1]);
    pos[2].xyz = vec3(intersection[2].xy, z[2]);

    for(int i=0; i<3; i++){
        gl_Position = pos[i];
        dilatedPos = pos[i].xy;
        worldPosFrag = vec3(viewProjectionI * pos[i]);
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