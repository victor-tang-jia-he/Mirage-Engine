#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 viewPos;
uniform bool debugNoLight;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float innerCutoff;  
    float outerCutoff;  
    float constant;
    float linear;
    float quadratic;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 CalcLight(vec3 lightDir, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular) {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

    vec3 texColor = texture(tex, TexCoord).rgb;

    vec3 ambient = lightAmbient * texColor;
    vec3 diffuse = lightDiffuse * diff * texColor;
    vec3 specular = lightSpecular * spec;

    return ambient + diffuse + specular;
}

void main() {
    if (debugNoLight) {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0); // Magenta debug color
        return;
    }

    vec3 result = vec3(0.0);

    // === Directional Light ===
    vec3 lightDir = normalize(-dirLight.direction);
    result += CalcLight(lightDir, dirLight.ambient, dirLight.diffuse, dirLight.specular);

    // === Point Light ===
    vec3 toPoint = pointLight.position - FragPos;
    float dist = length(toPoint);
    vec3 pointDir = normalize(toPoint);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * dist * dist);
    result += CalcLight(pointDir, pointLight.ambient, pointLight.diffuse, pointLight.specular) * attenuation;

    // === Spot Light ===
    vec3 toSpot = FragPos - spotLight.position; 
    vec3 spotDir = normalize(spotLight.direction);
    float theta = dot(normalize(toSpot), spotDir);

    float epsilon = spotLight.innerCutoff - spotLight.outerCutoff;
    float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0, 1.0);

    float spotDist = length(toSpot);
    float spotAtten = 1.0 / (spotLight.constant + spotLight.linear * spotDist + spotLight.quadratic * spotDist * spotDist);

    vec3 spotLightDir = normalize(-toSpot);
    result += CalcLight(spotLightDir, spotLight.ambient, spotLight.diffuse, spotLight.specular) * intensity * spotAtten;

    FragColor = vec4(result, 1.0);
}
