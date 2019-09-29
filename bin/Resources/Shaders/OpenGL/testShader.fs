#version 330

in vec2 texCoord0;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
    vec3 FragPos = texture(gPosition, texCoord0).rgb;
    vec3 Normal = texture(gNormal, texCoord0).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, texCoord0).rgb;

	gl_FragColor = vec4(Diffuse, 1) * clamp(dot(-vec3(-1, -1, -1), Normal), 0, 1);;
}