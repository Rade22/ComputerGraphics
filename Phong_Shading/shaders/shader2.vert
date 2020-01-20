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
varying vec3 Normal;
varying vec3 Fragpos;

varying vec3 l_pos[NUM_LIGHTS];
varying vec3 l_col[NUM_LIGHTS];

void main()
{

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	//color = vec3(0.0f, 1.0f, 0.0f);
	
	//my code
	Fragpos = vec3(model * vec4(vPositionModel,1.0));
	Normal = vec3(invtanModel * vec4(vNormalModel,0.0));
	for (int i = 0; i < NUM_LIGHTS; i++){
		l_pos[i] = lights[i].position;
		l_col[i] = lights[i].color;
	}
}
