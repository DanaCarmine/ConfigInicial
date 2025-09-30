#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
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
uniform Light light2; // Luz nueva

uniform sampler2D texture_diffusse;

void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // --- LUZ 1 ---
    vec3 lightDir  = normalize(light.position - FragPos);
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir= reflect(-lightDir, norm);
    float spec     = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient   = light.ambient  * material.diffuse;
    vec3 diffuse   = light.diffuse  * diff * material.diffuse;
    vec3 specular  = light.specular * spec * material.specular;

    // --- LUZ 2 ---
    vec3 lightDir2   = normalize(light2.position - FragPos);
    float diff2      = max(dot(norm, lightDir2), 0.0);
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2      = pow(max(dot(viewDir, reflectDir2), 0.0), material.shininess);

    vec3 ambientB   = light2.ambient  * material.diffuse;
    vec3 diffuseB   = light2.diffuse  * diff2 * material.diffuse;
    vec3 specularB  = light2.specular * spec2 * material.specular;

    // Suma de ambas luces
    vec3 result = (ambient + diffuse + specular) + (ambientB + diffuseB + specularB);

    // Textura
   // color = vec4(result, 1.0) * texture(texture_diffusse, TexCoords);
 
}
