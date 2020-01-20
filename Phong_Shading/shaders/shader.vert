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
out vec3 Normal;
out vec3 Fragpos;

void main()
{

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	//color = vec3(1.0f, 0.0f, 0.0f);
	
	//my code
	Fragpos = vec3(model * vec4(vPositionModel,1.0));
	Normal = vec3(invtanModel * vec4(vNormalModel,0.0));
	
	vec3 L,R,E;
	vec3 I;
	vec3 spec, diff;
	
	float alpha, beta, gamma;
	float Xa, Xb, Xc, Ya, Yb, Yc;
	Xa = Fragpos[0].x;
	Xb = Fragpos[1].x;
	Xc = Fragpos[2].x;
	Ya = Fragpos[0].y;
	Yb = Fragpos[1].y;
	Yc = Fragpos[2].y;
	float neg = -1.0f;


	alpha = (neg*(x - Xb)*(Yc - Yb) + (y - Yb)*(Xc - Xb)) /
		(neg*(Xa - Xb)*(Yc - Yb) + (Ya - Yb)*(Xc - Xb));

	beta = (neg*(x - Xc)*(Ya - Yc) + (y - Yc)*(Xa - Xc)) /
		(neg*(Xb - Xc)*(Ya - Yc) + (Yb - Yc)*(Xa - Xc));

	gamma = 1.0f - alpha - beta;
	
	for(int j = 0; j < 3; j++){
		for(int i = 0; i < NUM_LIGHTS; i++){
			L[j] = normalize(lights[i].position - Fragpos[j]);
			E[j] = normalize(eye - Fragpos[j]);
			R[j] = normalize(2*(dot(L,Normal[j]))*(Normal[j]-L));
		
			float dot1 = max(dot(L,Normal[j]),0.0);
			float dot2 = pow(max(dot(R,E),0.0),s);
		
			spec = ks * dot2;
			diff = kd * dot1;
	
			I[j] += lights[i].color*diff + lights[i].color*spec;
	}
	
	I[j] = I[j] + ka;
	
	color = (alpha*I[0]) + (beta*I[1]) + (gamma*I[2]);
}
