#version 400

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;

out vec2 texCoord0;
out vec3 worldPos0;
out mat3 tbnMatrix0;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	
    worldPos0 = (model * vec4(position, 1.0)).xyz;
    texCoord0 = texCoord;
	
	vec3 n = normalize((model * vec4(normal, 0.0)).xyz);
	vec3 t = normalize((model * vec4(tangent, 0.0)).xyz);
	t = normalize(t - dot(t, n) * n);
	
	vec3 biTangent = cross(t, n);
	
	tbnMatrix0 = mat3(t, biTangent, n);
}