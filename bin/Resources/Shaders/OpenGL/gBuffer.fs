#version 400

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

in vec2 texCoord0;
in vec3 worldPos0;
in mat3 tbnMatrix0;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;

void main()
{    
    gPosition = worldPos0;
    gNormal = normalize(tbnMatrix0 * (255.0 / 128.0 * texture2D(normalMap, texCoord0).xyz - 1));
    gAlbedoSpec = texture2D(albedoMap, texCoord0);
}