#version 460
const uint maxIterations = 10000;
const uint colorPeriod = 24u;
const vec3 col1 = vec3(1.f, 0, 0);
const vec3 col2 = vec3(1.0, 1.0, 0.f);

uniform vec2 resolution;
uniform vec2 offset;
uniform float zoom;

out vec4 fragColor;
in vec4 gl_FragCoord;

void main()
{
	vec2 C = ((gl_FragCoord.xy / resolution) * 2) - 1;
	C += offset;
	C *= zoom;
	vec2 z = vec2(0,0);
	
	float rSquare = 0;
	uint it = 0;
	//Z^2 + C
	while(rSquare < 4 && it < maxIterations) 
	{
		const float zReal = (z.x*z.x - z.y*z.y) + C.x;
		const float zIm = (2*z.x*z.y) + C.y;
		rSquare = zReal * zReal + zIm * zIm;
		z = vec2(zReal, zIm);
		++it;
	}
	fragColor = vec4(mix(col1, col2, float(it % colorPeriod)/float(colorPeriod)) * vec3(it < maxIterations), 1.0);
}