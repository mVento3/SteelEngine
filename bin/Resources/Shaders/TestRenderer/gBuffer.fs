#version 400

out vec4 gAlbedoSpec;

in vec3 outPosition;

void main()
{
	gAlbedoSpec = vec4(outPosition, 1);
}