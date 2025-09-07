#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vWorldPos;  // posici�n en espacio mundo (para derivadas en FS)

void main() {
    vec4 wp = model * vec4(position, 1.0);
    vWorldPos = wp.xyz;
    gl_Position = projection * view * wp;
}
