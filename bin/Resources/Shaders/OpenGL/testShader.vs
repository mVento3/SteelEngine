#version 120

attribute vec3 position;
attribute vec2 texCoord;

varying vec2 texCoord0;

void main()
{
	texCoord0 = texCoord;
	gl_Position = vec4(position, 1.0);
}