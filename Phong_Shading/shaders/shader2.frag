#version 120

#define NUM_LIGHTS 2

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;
uniform vec3 eye;

varying vec3 color;

varying vec3 Normal;
varying vec3 Fragpos;

varying vec3 l_pos[NUM_LIGHTS];
varying vec3 l_col[NUM_LIGHTS];

void main()
{
	//gl_FragColor = vec4(color, 1.0f);
	
	//my code
	vec3 L,R,E;
	vec3 I;
	vec3 spec, diff;
	
	for(int i = 0; i < NUM_LIGHTS; i++){
		L = normalize(l_pos[i] - Fragpos);
		E = normalize(eye - Fragpos);
		R = normalize(2*(dot(L,Normal))*Normal-L);
		
		spec = ks * pow(max(dot(R,E),0.0f),s);
		diff = kd * max(dot(L,Normal),0.0f);
	
		I += l_col[i]*(diff + spec);
	}
	
	I = I + ka;
	
	gl_FragColor = vec4(I,1.0f);
}