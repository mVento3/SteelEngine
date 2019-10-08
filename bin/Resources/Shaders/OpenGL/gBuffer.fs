#version 400

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

in vec2 texCoord0;
in vec3 worldPos0;
in mat3 tbnMatrix0;
in vec4 shadowMapCoord0;
in vec4 shadowMapCoord1;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D dispMap;
uniform sampler2D directionalLightShadowMap;
uniform sampler2D spotLightShadowMap;

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
	bool shadowMap;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation attenuation;
	vec3 position;
	float range;
};

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutoff;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

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

vec4 calcPointLight(PointLight light, vec3 normal)
{
	vec3 lightDirection = worldPos0 - light.position;
	float distanceToPoint = length(lightDirection);
	
	if(distanceToPoint > light.range)
	{
		return vec4(0, 0, 0, 0);
	}
	
	lightDirection = normalize(lightDirection);
	
	vec4 color = calcLight(light.base, lightDirection, normal);
	float attenuation = light.attenuation.constant +
						light.attenuation.linear * distanceToPoint +
						light.attenuation.exponent * (distanceToPoint * distanceToPoint);
	
	return color * (1.0 / attenuation);
}

vec4 calcSpotLight(SpotLight light, vec3 normal)
{
	vec3 lightDirection = normalize(worldPos0 - light.pointLight.position);
	float spotFactor = dot(lightDirection, light.direction);
	vec4 color = vec4(0, 0, 0, 0);
	
	if(spotFactor > light.cutoff)
	{
		color = calcPointLight(light.pointLight, normal) * (1.0 - (1.0 - spotFactor) / (1.0 - light.cutoff));
	}
	
	return color;
}

float linstep(float low, float height, float v)
{
	return clamp((v - low) / (height - low), 0.0, 1.0);
}

float sampleShadowMap(sampler2D shadowMap, vec2 coord, float compare)
{
	vec2 moments = texture2D(shadowMap, coord).xy;
	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, 0.00002);
	float d = compare - moments.x;
	float pMax = linstep(0.2, 1.0, variance / (variance + d * d));
	
	return min(max(p, pMax), 1.0);
}

float calcShadowAmount(sampler2D shadowMap, vec4 initialShadowMapCoord)
{
	vec3 shadowMapCoord = (initialShadowMapCoord.xyz / initialShadowMapCoord.w) * 0.5 + 0.5;
	
	return sampleShadowMap(shadowMap, shadowMapCoord.xy, shadowMapCoord.z);
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
	
	totalLight += calcDirectionalLight(directionalLight, gNormal) * calcShadowAmount(directionalLightShadowMap, shadowMapCoord1);
	// totalLight += calcPointLight(pointLight, gNormal);
	totalLight += calcSpotLight(spotLight, gNormal) * calcShadowAmount(spotLightShadowMap, shadowMapCoord0);

	
	gAlbedoSpec = color * totalLight;
}