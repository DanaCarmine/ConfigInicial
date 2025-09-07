#version 330 core
in vec3 vWorldPos;
out vec4 FragColor;

uniform vec3 uFaceColors[6];

void main() {
    // Normal de la superficie en espacio mundo vía derivadas de pantalla
    vec3 dpx = dFdx(vWorldPos);
    vec3 dpy = dFdy(vWorldPos);
    vec3 N = normalize(cross(dpx, dpy));

    // Elegir eje dominante de la normal
    vec3 a = abs(N);
    int faceId;
    if (a.x > a.y && a.x > a.z) {
        faceId = (N.x > 0.0) ? 0 : 1;  // +X / -X
    } else if (a.y > a.z) {
        faceId = (N.y > 0.0) ? 2 : 3;  // +Y / -Y
    } else {
        faceId = (N.z > 0.0) ? 4 : 5;  // +Z / -Z
    }

    FragColor = vec4(uFaceColors[faceId], 1.0);
}
