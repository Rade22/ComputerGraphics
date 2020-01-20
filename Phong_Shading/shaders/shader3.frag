#version 120

uniform vec3 eye;

varying vec3 color;
varying vec3 Fragpos;
varying vec3 Normal;

void main()
{
	//gl_FragColor = vec4(color, 1.0f);
	
	//my code
	vec3 E = normalize(eye - Fragpos);
	
	vec3 I;
	
	float dot1 = dot(Normal,E);
	
	if(dot1 <= 0.23f && dot1 >= -0.23f){
		I = vec3(1.0,1.0,1.0);
	}
	else{
		I = vec3(0.0,0.0,0.0);
	}
	
	gl_FragColor = vec4(I,1.0f);
}