#version 400

in vec3 position;

out vec3 outPosition;

void main()
{
	gl_Position = vec4(position, 1.0);
	
	outPosition = position;
}