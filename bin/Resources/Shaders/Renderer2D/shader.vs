#version 400

in vec3 position;
in vec4 color;
in vec2 texCoord;
in float texID;

out vec4 colorOut;
out vec2 texCoordOut;
out float texID_Out;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	colorOut = color;
	texCoordOut = texCoord;
	texID_Out = texID;
    gl_Position = projection * view * vec4(position, 1.0);
}