#version 400

in vec4 colorOut;
in vec2 texCoordOut;
in float texID_Out;

uniform sampler2D textures[2];

void main()
{
	int id = int(texID_Out);

	gl_FragColor = texture(textures[id], texCoordOut) * colorOut;
}