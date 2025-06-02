#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Material properties - these match what Mesh::draw() sets
uniform vec3 materialAmbient;   // Set by mesh.material.ambient
uniform vec3 materialDiffuse;   // Set by mesh.material.diffuse
uniform vec3 materialSpecular;  // Set by mesh.material.specular
uniform float materialShininess; // Set by mesh.material.shininess

// Model color from Model::draw() - can be used as fallback
uniform vec3 DiffuseColor;

// Light properties
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

// Light attenuation control
uniform float lightRadius; // How far the light reaches
uniform bool useAttenuation; // Whether to use distance falloff

// Texture properties
uniform bool useTexture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // Normalize the normal vector
    vec3 norm = normalize(Normal);
    
    // Calculate light direction and distance
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // Calculate attenuation
    float attenuation = 1.0;
    if (useAttenuation) {
         float distance = length(lightPos - FragPos);
        attenuation = max(0.0, 1.0 - (distance / lightRadius));
    }
    
    // Determine material colors
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    
    if (useTexture) {
        // Use textures if available
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        ambientColor = vec3(texColor) * 0.1; // Reduced ambient
        diffuseColor = vec3(texColor);
        
        // Use specular map if available
        vec4 specTexColor = texture(texture_specular1, TexCoords);
        specularColor = vec3(specTexColor);
        shininess = 32.0;
    } else {
        // Use material colors from MTL file (set by Mesh::draw())
        // Many MTL files have Ka values of 1.0,1.0,1.0 which is too bright
        ambientColor = materialAmbient * 0.1; // Much lower ambient
        
        // Use material diffuse, fallback to DiffuseColor if material diffuse is black/zero
        if (length(materialDiffuse) > 0.001) {
            diffuseColor = materialDiffuse;
        } else {
            diffuseColor = DiffuseColor; // Fallback to model color
        }
        
        // Debug: Force a fallback if diffuse is still black
        if (length(diffuseColor) < 0.001) {
            diffuseColor = vec3(0.5, 0.5, 0.5); // Gray fallback for debugging
        }
        
        specularColor = materialSpecular;
        shininess = materialShininess;
    }
    
    // Ambient lighting - higher base level so distant objects aren't too dark
    vec3 ambient = 0.1 * ambientColor * lightColor;
    
    // Diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor * attenuation;
    
    // Specular lighting (Blinn-Phong model)
    vec3 specular = vec3(0.0);
    if (length(specularColor) > 0.001) {
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
        specular = spec * specularColor * lightColor * attenuation;
    }
    
    // Combine all lighting components
    vec3 result = ambient + diffuse + specular;
    
    
    // Output the final calculated lighting result
    FragColor = vec4(diffuseColor, 1.0);

    // "Fake" lighting based on distance to camera
    float depth = length(viewPos - FragPos);
    float depthFade = clamp(depth / 20.0, 0.0, 1.0);

    // Posterize the base diffuse color
    vec3 posterized = floor(diffuseColor * 4.0) / 4.0;

    // Darken further objects more strongly
    vec3 finalColor = mix(posterized, posterized * 0.1, depthFade);

    FragColor = vec4(finalColor, 1.0);
}