#version 400

in vec2 texCoord0;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
    vec3 position = texture(gPosition, texCoord0).rgb;
    vec3 normal = texture(gNormal, texCoord0).rgb;
	vec4 textureColor = texture(gAlbedoSpec, texCoord0);

	gl_FragColor = textureColor;
}