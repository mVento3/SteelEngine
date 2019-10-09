#version 400

void main()
{
	float depth = gl_FragCoord.z;
	float dX = dFdx(depth);
	float dY = dFdy(depth);
	float moment2 = depth * depth + 0.25 * (dX * dX + dY * dY);

	gl_FragColor = vec4(depth, moment2, 0, 0);
}