#version 330 core
out vec4 fragColor;

uniform vec3 objCol;
uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

in vec3 Normal;
in vec3 FragPos; 
in vec3 nom;
in vec2 texCoords;
void main() 
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diffuseAng = max(dot(norm,lightDir),0.0f);

    vec3 lightCo = lightCol;
    vec3 diffuseColor = diffuseAng * lightCo;

    float ambientStrength = 0.1f;
    vec3 ambientColor = ambientStrength * lightCo;

    float specularStrength = 0.2f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specularFactor = pow(max(dot(viewDir,reflectDir),0.0f),32);
    vec3 specularColor = specularFactor * lightCo;

    gl_FragColor = texture(texture_diffuse1,texCoords) * vec4((ambientColor + diffuseColor + specularColor) ,1.0f );
    // gl_FragColor = vec4(vec3(0.8)*(ambientColor + diffuseColor + specularColor) ,1.0f );
}