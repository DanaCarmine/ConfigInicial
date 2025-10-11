#version 330 core

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;   // usa valores tipo 16, 32, 64...
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform Light light2;

// Samplers que usa el loader del Model (estilo LearnOpenGL)
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // Muestras de texturas (si no hay textura, suelen devolver 0)
    vec3  texDiffuse  = texture(texture_diffuse1,  TexCoords).rgb;
    vec3  texSpecular = texture(texture_specular1, TexCoords).rgb;

    // Fallbacks: si la textura es negra (0), usamos el material.* definido por uniform
    vec3 baseColor   = max(texDiffuse,  material.diffuse);
    vec3 specColor   = max(texSpecular, material.specular);

    // ---- Luz 1 ----
    vec3 L1     = normalize(light.position - FragPos);
    float diff1 = max(dot(N, L1), 0.0);
    vec3  R1    = reflect(-L1, N);
    float spec1 = pow(max(dot(V, R1), 0.0), material.shininess);

    // ---- Luz 2 ----
    vec3 L2     = normalize(light2.position - FragPos);
    float diff2 = max(dot(N, L2), 0.0);
    vec3  R2    = reflect(-L2, N);
    float spec2 = pow(max(dot(V, R2), 0.0), material.shininess);

    // Componentes
    vec3 ambient  = (light.ambient  + light2.ambient)                 * baseColor;
    vec3 diffuse  = (light.diffuse  * diff1 + light2.diffuse * diff2) * baseColor;
    vec3 specular = (light.specular * spec1 + light2.specular * spec2) * specColor;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
