#version 400

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

in vec2 texCoord0;
in vec3 worldPos0;
in mat3 tbnMatrix0;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D dispMap;

uniform vec3 ambientLight;
uniform vec3 baseColor;

uniform float dispMapScale;
uniform float dispMapOffset;

uniform float specularIntensity;
uniform float specularPower;

uniform vec3 eyePosition;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform DirectionalLight directionalLight;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(base.color, 1) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(eyePosition - worldPos0);
		vec3 reflectionDirection = normalize(reflect(direction, normal));
		float specularFactor = dot(directionToEye, reflectionDirection);
		
		specularFactor = pow(specularFactor, specularPower);
		
		if(specularFactor > 0)
		{
			specularColor = vec4(base.color, 1) * specularIntensity * specularFactor;
		}
	}
	
	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 normal)
{
	return calcLight(light.base, -light.direction, normal);
}

void main()
{
	vec3 directionToEye = normalize(eyePosition - worldPos0);
	vec2 texCoord = texCoord0 + (directionToEye * tbnMatrix0).xy * (texture2D(dispMap, texCoord0).r * dispMapScale + dispMapOffset);
	
	if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
		discard;
	
    gPosition = worldPos0;
    gNormal = normalize(tbnMatrix0 * (2.0 * texture2D(normalMap, texCoord).xyz - 1.0));
	
	vec4 textureColor = texture2D(albedoMap, texCoord);
	vec4 totalLight = vec4(ambientLight, 1);
	vec4 color = vec4(baseColor, 1);
	
	if(textureColor != vec4(0, 0, 0, 0))
	{
		color *= textureColor;
	}
	
	totalLight += calcDirectionalLight(directionalLight, gNormal);
	
	gAlbedoSpec = color * totalLight;
}