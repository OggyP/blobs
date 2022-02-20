#version 130

uniform vec2 points[100];
uniform vec2 resolution;
uniform float pointsMag[100];

void main(void) {
	float pointsValue = 0.0;
	for(int i = 0; i < 100; i++) {
		vec2 currentPoint = points[i];
		float distSquared = pow(gl_FragCoord.x - currentPoint.x, 2.0) + pow((resolution.y - gl_FragCoord.y) - currentPoint.y, 2.0);
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