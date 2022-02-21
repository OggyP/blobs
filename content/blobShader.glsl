#version 130

#define POINTSAMOUNT 50

uniform vec2 points[POINTSAMOUNT];
uniform vec2 resolution;
uniform float pointsMag[POINTSAMOUNT];

void main(void) {
	float pointsValue = 0.0;
	for(int i = 0; i < POINTSAMOUNT; i++) {
		vec2 currentPoint = points[i];
		vec2 dif = vec2(gl_FragCoord.x - currentPoint.x, (resolution.y - gl_FragCoord.y) - currentPoint.y);
		float distSquared = dif.x * dif.x + dif.y * dif.y;
		distSquared /= pointsMag[i];
		if(distSquared < 800.0)
			pointsValue += pow(0.990, distSquared - 270.0);
	}

	if(pointsValue >= 5.0)
		gl_FragColor = vec4(1);
	// if(pointsValue >= 5.0)
	// 	gl_FragColor = vec4(1);
	else
		gl_FragColor = vec4(0, 0, 0, 1);;
}