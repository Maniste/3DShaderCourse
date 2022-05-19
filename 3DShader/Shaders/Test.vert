#version 330 core
uniform float u_time;
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal; 
layout(location = 2) in vec2 a_TexCoord;

out vec3 f_Position;
out vec3 f_Normal;
out vec2 f_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);

	f_Normal = a_Normal;
	f_Position = a_Position;
	f_TexCoord = a_TexCoord;
}