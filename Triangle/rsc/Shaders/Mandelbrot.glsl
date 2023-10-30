#version 460
const vec3 col1 = vec3(1.f, 0, 0);
const vec3 col2 = vec3(1.0, 1.0, 0.f);

uniform uint maxIterations;
uniform uint colorPeriod;
uniform ivec2 resolution;
uniform double cLength;
uniform dvec2 center;

out vec4 fragColor;
in vec4 gl_FragCoord;

void main()
{
	const dvec2 inc = cLength / dvec2(resolution);
	dvec2 C = (center - cLength * 0.5lf) + (gl_FragCoord.xy * inc); 
	dvec2 z = dvec2(0,0);
	
	double rSquare = 0;
	uint it = 0;
	//Z^2 + C
	while(rSquare < 4 && it < maxIterations) 
	{
		const double zReal = (z.x*z.x - z.y*z.y) + C.x;
		const double zIm = (2*z.x*z.y) + C.y;
		rSquare = zReal * zReal + zIm * zIm;
		z = dvec2(zReal, zIm);
		++it;
	}
	fragColor = vec4(mix(col1, col2, float(it % colorPeriod)/float(colorPeriod)) * vec3(it < maxIterations), 1.0);
}