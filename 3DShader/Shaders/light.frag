#version 330 core

//inputs
in vec3 f_Normal;
in vec3 f_World;

//uniforms
uniform vec3 u_DirectionalLight;
uniform vec3 u_EyePosition;

struct Point_Light
{
	vec3 color;
	vec3 position;
	float intensity;
};

uniform Point_Light u_PointLight[4];


//light data
const vec3 AMBIENT = vec3(0.1f, 0.1f, 0.5f);
const float AMBIENT_STRENGHT = 0.15;
const float SPEC_EXPONENT = 32.0;
const float SPEC_INTENSITY = 0.8;


//output
out vec4 o_Color;

vec3 light_func(vec3 light_dir, vec3 color,float intensity)
{
	//diffuse lightning
	float diffuse = dot(f_Normal, -light_dir);
	diffuse = clamp(diffuse, 0.0, 1.0);

		//Spec light
	vec3 eye_dir = normalize(u_EyePosition - f_World);
	vec3 middle = normalize(eye_dir + -light_dir);

	float specular = dot(f_Normal, middle);
	specular = max(specular, 0.0);
	specular = pow(specular, SPEC_EXPONENT) * SPEC_INTENSITY;

	return color * (diffuse + specular) * intensity;
}

void main()
{
	vec3 light_clr = vec3(0.0);
	//Add directionl Light
	//light_clr += light_func(u_DirectionalLight,vec3(1.0), 0.6);

	//add point light
	for(int i = 0; i < u_PointLight.length; i++)
	{
		Point_Light light = u_PointLight[i];
		vec3 point_light_dir = normalize(f_World - light.position);
		float dist = 1.f / length(f_World - light.position);
		light_clr += light_func(point_light_dir, light.color,light.intensity) * dist;
	}
	//Ambient lightning
	vec3 ambient = AMBIENT * AMBIENT_STRENGHT;

	o_Color = vec4(ambient + light_clr, 1.0);
}