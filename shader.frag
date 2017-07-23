#version 330 core
// This is a sample fragment shader.
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

in vec3 FragPos;
in vec3 Normal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform float toggle;

vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 direction, vec3 ambient_c, vec3 diffuse_c, vec3 specular_c);

void main()
{
	if(toggle == 1.0)
	{
		vec3 norm = normalize(Normal);
		
		vec3 viewDir = normalize(viewPos - FragPos);

		vec3 direction = vec3(0.0, 0.0, -5.0);
		vec3 amb = vec3(0.1, 0.1, 0.1);
		vec3 dif = vec3(0.6, 0.6, 0.6);
		vec3 spec = vec3(1.0, 1.0, 1.0);

		vec3 result = CalcDirLight(norm, viewDir, direction, amb, dif, spec);

		direction = vec3(0.0, 5.0, 0.0);
		result += CalcDirLight(norm, viewDir, direction, amb, dif, spec);

		color = vec4(result, 1.0);
	} else 
	{
		color = vec4(0.8);
	}
}


vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 direction, vec3 ambient_c, vec3 diffuse_c, vec3 specular_c)
{
    vec3 lightDir = normalize(-direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = ambient_c * material.ambient;
    vec3 diffuse = diffuse_c * (diff * material.diffuse);
    vec3 specular = specular_c * (spec * material.specular);
    return (ambient + diffuse + specular);
} 

/*
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
	float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
 
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

	float theta = dot(-lightDir, normalize(light.direction));
  	float epsilon   = light.cutoff - light.outcutoff;
    float intensity = clamp((theta - light.outcutoff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
    specular *= intensity;

	float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.quadratic * (distance * distance));    

    diffuse  *= attenuation;
    specular *= attenuation;   
            
    return (ambient + diffuse + specular);
} */