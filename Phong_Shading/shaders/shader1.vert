#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

uniform mat4 model;
uniform mat4 invtanModel;
uniform mat4 view;
uniform mat4 projection;

struct lightStruct
{
	vec3 position;
	vec3 color;
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;
uniform vec3 eye;

varying vec3 color;


void main()
{

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0f);
	//color = vec3(1.0f, 0.0f, 0.0f);
	
	//my code
	vec3 Fragpos = vec3(model * vec4(vPositionModel,1.0f));
	vec3 Normal = normalize(vec3(invtanModel * vec4(vNormalModel,0.0f)));
	
	vec3 L,R,E;
	vec3 I;
	vec3 spec, diff;
	vec3 sum;
	
	for(int i = 0; i < NUM_LIGHTS; i++){
		L = normalize(lights[i].position - Fragpos);
		E = normalize(eye - Fragpos);
		R = normalize(2.0f*dot(L,Normal)*Normal-L);
		
		
		diff = kd * max(0.0f,dot(L,Normal));
		spec = ks * pow(max(0.0f,dot(R,E)),s);
		sum = diff + spec;
		
		I += lights[i].color * (sum);
	
	}
	
	I += ka;
	
	color = I;
}
