#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Material properties from MTL file
uniform vec3 materialAmbient;   // Ka from MTL
uniform vec3 materialDiffuse;   // Kd from MTL
uniform vec3 materialSpecular;  // Ks from MTL
uniform float materialShininess; // Ns from MTL

// Light properties
uniform vec3 lightPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0); // Default white light
uniform vec3 viewPos;   

// If we're using textures or flat colors
uniform bool useTexture = false;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // Debug outputs - uncomment to see raw material values
    // FragColor = vec4(materialDiffuse, 1.0); return;
    // FragColor = vec4(materialAmbient, 1.0); return;
    
    // Normalize the normal vector
    vec3 norm = normalize(Normal);
    
    // Calculate light direction and view direction
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // Determine ambient, diffuse, and specular colors
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    
    if (useTexture) {
        // Use textures if available
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        ambientColor = vec3(texColor) * 0.2;
        diffuseColor = vec3(texColor);
        
        // Use specular map if available, otherwise default
        vec4 specTexColor = texture(texture_specular1, TexCoords);
        specularColor = vec3(specTexColor);
        shininess = 32.0;
    } else {
        // Use material colors
        // Scale down ambient - the Ka 1.0,1.0,1.0 in MTL is too bright
        ambientColor = materialAmbient * 0.2;
        
        // Use material diffuse or fallback to DiffuseColor
        // Don't check length - zero is valid for black materials
        diffuseColor = materialDiffuse;
        
        // Use material specular - zero specular (as in your MTL) means no specular highlight
        specularColor = materialSpecular;
        shininess = max(materialShininess, 1.0); // Avoid 0 shininess
    }
    
    // Ambient lighting (reduced strength to prevent washing out)
    vec3 ambient = 0.2 * ambientColor * lightColor;
    
    // Diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor;
    
    // Specular lighting (Blinn-Phong model)
    // Only calculate if specular is non-zero
    vec3 specular = vec3(0.0);
    if (length(specularColor) > 0.001) {
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
        specular = spec * specularColor * lightColor;
    }
    
    // Apply a small ambient occlusion factor to create some depth
    float ao = 0.95; // Subtle ambient occlusion
    
    // Combine results
    vec3 result = (ambient + diffuse) * ao + specular;
    
    // Gamma correction for better visual appearance
    result = pow(result, vec3(1.0/2.2));
    
    FragColor = vec4(result, 1.0);
}