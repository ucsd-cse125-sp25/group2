#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Material properties
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

// Model color fallback
uniform vec3 DiffuseColor;

// Multi-light support
#define MAX_LIGHTS 8
uniform int numLights;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform float lightRadii[MAX_LIGHTS];
uniform bool useAttenuation[MAX_LIGHTS];

// Camera/view properties
uniform vec3 viewPos;

// Global lighting controls
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform bool isTransparent;

// Texture properties
uniform bool useTexture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

// Function to calculate lighting contribution from a single light
vec3 calculateLighting(int lightIndex, vec3 norm, vec3 fragPos, vec3 viewDir, 
                      vec3 diffuseColor, vec3 specularColor, float shininess) {
    vec3 lightPos = lightPositions[lightIndex];
    vec3 lightColor = lightColors[lightIndex];
    float lightRadius = lightRadii[lightIndex];
    bool useAtten = useAttenuation[lightIndex];
    
    // Calculate light direction and distance
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // Calculate attenuation
    float attenuation = 1.0;
    if (useAtten) {
        float distance = length(lightPos - fragPos);
        // Smooth attenuation that reaches zero at lightRadius
        attenuation = max(0.0, 1.0 - (distance / lightRadius));
        attenuation = attenuation * attenuation; // Quadratic falloff
    }
    
    // Diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor * attenuation * diffuseStrength;
    
    // Specular lighting (Blinn-Phong)
    vec3 specular = vec3(0.0);
    if (length(specularColor) > 0.001) {
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
        specular = spec * specularColor * lightColor * attenuation * specularStrength;
    }
    
    return diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Determine material colors
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    
    if (useTexture) {
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        ambientColor = vec3(texColor) * 0.1;
        diffuseColor = vec3(texColor);
        
        vec4 specTexColor = texture(texture_specular1, TexCoords);
        specularColor = vec3(specTexColor);
        shininess = 32.0;
    } else {
        ambientColor = materialAmbient * 0.1;
        
        if (length(materialDiffuse) > 0.001) {
            diffuseColor = materialDiffuse;
        } else {
            diffuseColor = DiffuseColor;
        }
        
        if (length(diffuseColor) < 0.001) {
            diffuseColor = vec3(0.7, 0.7, 0.7);
        }
        
        specularColor = materialSpecular;
        shininess = materialShininess;
    }
    
    // Global ambient lighting
    vec3 ambient = ambientStrength * ambientColor;
    
    // Calculate lighting from all active lights
    vec3 totalLighting = vec3(0.0);
    for (int i = 0; i < min(numLights, MAX_LIGHTS); i++) {
        totalLighting += calculateLighting(i, norm, FragPos, viewDir, 
                                         diffuseColor, specularColor, shininess);
    }
    
    // Combine ambient and light contributions
    vec3 result = ambient + totalLighting;
    
    // Optional depth fade for distant objects
    float depth = length(viewPos - FragPos);
    float depthFade = clamp(depth / 50.0, 0.0, 0.3);
    vec3 finalColor = mix(result, result * 0.7, depthFade);
    
    float alpha = isTransparent ? 0.5 : 1.0;
    FragColor = vec4(finalColor, alpha);
}