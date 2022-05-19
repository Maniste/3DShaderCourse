#version 330 core

in vec3 f_Position;
in vec3 f_Normal;
in vec2 f_TexCoord;

uniform sampler2D u_Sampler0;
uniform sampler2D u_Sampler1;

uniform sampler2D u_Sampler;
uniform float u_time;
uniform vec4 u_Color;


out vec4 o_Color;

void main()
{
	/*
	vec4 color_A = vec4(0.5, 0.2, 0.8, 1.0);
	vec4 color_B = vec4(0.1, 0.9, 0.3, 1.0);

	o_Color = mix(color_A, color_B, sin(u_time) * 0.5f+ 0.5);
	*/

	//o_Color = vec4(f_Color, 1f);

	vec4 col_a = texture(u_Sampler0, f_TexCoord);
	vec4 col_b = texture(u_Sampler1, f_TexCoord);
	o_Color = mix(col_a, col_b, sin((u_time + f_Position.x) * 5f) * 0.5 + 0.5);

	o_Color = vec4(f_Normal, 1.0f);
	o_Color = texture(u_Sampler0, f_TexCoord);
}