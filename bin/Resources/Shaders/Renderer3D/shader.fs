#version 400

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    float shininess;
}; 

in vec2 texCoordOut;
in vec3 normalOut;

uniform Material material;
uniform DirectionalLight directionalLight;

uniform sampler2D texture;

uniform vec3 eyePosition;

vec4 calcDirectionalLight(DirectionalLight light, vec3 normal)
{
	vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(eyePosition, reflectDir), 0.0), material.shininess);
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
    return vec4(ambient + diffuse + specular, 1.0);
}

void main()
{
	vec4 result = texture2D(texture, texCoordOut);

	result += calcDirectionalLight(directionalLight, normalOut);

	gl_FragColor = result;
}