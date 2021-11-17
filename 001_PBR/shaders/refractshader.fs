#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    vec3 V = normalize(cameraPos - Position);
    vec3 N = normalize(Normal);
    float ratio = 1.00 / 1.52;
    vec3 R = refract(-V, N, ratio);
        vec3 color = texture(skybox, R).rgb;
    color = color / (vec3(1.0) + color);
    color = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(color, 1.0); 
}
