#version 400

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoordOut;
out vec3 normalOut;

void main()
{
	texCoordOut = texCoord;
	normalOut = normalize((model * vec4(normal, 0.0)).xyz);
    gl_Position = projection * view * model * vec4(position, 1.0);
}