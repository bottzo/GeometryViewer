const uint maxIterations = 10000;
uniform uvecn2 resolution;

void main(out vec4 fragColor, in vec2 gl_FragCoord)
{
	vec2 C = ((gl_FragCoord.xy / resolution) * 2) - 1;
	vec2 z = vec2(0,0);
	
	float rSquare = 0;
	uint it = 0;
	//Z^2 + C
	while(rSqure < 4 && it < maxItarations) {
		const float zReal = (z.x*z.x - z.y*z.y) + C.x;
		const float zIm = (2*z.x*z.y) + C.y;
		rSquare = zReal * zReal + zIm * zIm;
		z = uvecn2(zReal, zIm)
		++it;
	}	
}