#version 400

in vec2 outTexCoord;

out vec4 fragColor;

uniform vec3 color;
uniform sampler2D brick;

void main()
{
	fragColor = vec4(color, 1) * texture(brick, outTexCoord);
}