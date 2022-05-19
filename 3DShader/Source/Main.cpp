#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "mesh.h"
#include "global.h"

using namespace glm;

void Handel_Key_Event(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{

	if (action != GLFW_PRESS)
		return;
	
	if (key == GLFW_KEY_ESCAPE)
		glfwDestroyWindow(window);

}

void Handle_Mouse_Events(GLFWwindow* window, int button, int action, int modifiers)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cam_control = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cam_control = false;
		}
	}
}


void handle_mouse_pos(GLFWwindow* _window, double mouse_x, double mouse_y)
{
	mouse_position = vec2(mouse_x, mouse_y);
}

GLuint load_texture(const char* path)
{
	//Textures
	int width, height;
	int channels;
	stbi_uc* tex_data = stbi_load(path, &width, &height, &channels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return texture;
}

void handle_framebuffer_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cam.aspect = width / (float)height;
}

int main()
{
	glfwInit();
	_window = glfwCreateWindow(800, 600, "Bababooey", NULL, NULL);
	glfwMakeContextCurrent(_window);

	glewInit();

	glfwSetKeyCallback(_window, Handel_Key_Event);
	glfwSetMouseButtonCallback(_window, Handle_Mouse_Events);
	glfwSetCursorPosCallback(_window, handle_mouse_pos);
	glfwSetFramebufferSizeCallback(_window, handle_framebuffer_resize);


	glEnable(GL_DEPTH_TEST);

	//Load the shader program from our shader files
	Material mat = load_material("Shaders/test.vert", "Shaders/test.frag");
	materia_add_texture(&mat, load_texture("Assets/texture.png"));
	material_use(mat);

	Material mat2 = load_material("Shaders/light.vert", "Shaders/light.frag");
	materia_add_texture(&mat2, load_texture("Assets/texture2.png"));

	Material mat_pointlight = load_material("Shaders/light.vert", "Shaders/point_light.frag");

	// Mesh loading
	Mesh cube = mesh_load("Assets/Cube.obj");
	Mesh ball = mesh_load("Assets/Sphere.obj");
	Mesh monkeyHead = mesh_load("Assets/Monkey.obj");

	//set initial aspect ratio of cam
	int width, heigth;
	glfwGetFramebufferSize(_window, &width, &heigth);
	cam.aspect = width / (float)heigth;

	//for calculating dleta time and mouse delta

	float last_time = glfwGetTime();
	vec2 last_mouse_position = mouse_position;

	while (!glfwWindowShouldClose(_window))
	{

		//calculate the delta time
		float Time = glfwGetTime();
		delta_Time = Time - last_time;
		last_time = Time;

		//calculate mouse movement
		mouse_delta = mouse_position - last_mouse_position;
		last_mouse_position = mouse_position;

		//Update camera
		cam_update();

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		material_set("u_Time", Time);
		material_set("u_ViewProjection", cam_matrix());
	
		 // Draw nice meshes!
		Render_Data render_data;
		render_data.eye_position = cam.position;
		render_data.view_projection = cam_matrix();
		render_data.material = &mat2;
		render_data.directional_lght = normalize(vec3(-1.f, -1.f, -1.f));
		//render_data.directional_lght = normalize(vec3(sin(Time), -1.f, cos(Time)));

		//Make Pointlight
		render_data.point_light[0].position = vec3(cos(Time / 3.f), 0.f, sin(Time)) * 4.f;
		render_data.point_light[0].color = vec3(cos(Time), 0.f, -cos(Time));
		render_data.point_light[0].intensity = cos(Time * 2.f) + 1.5f;

		render_data.point_light[1].position = vec3(cos(Time * 1.6f), cos(Time * 0.7f), sin(Time * 1.6f)) * 4.f;
		render_data.point_light[1].color = vec3(1.f, 1.f, 0.f);
		render_data.point_light[1].intensity = 0.8f;


		mesh_draw(cube, render_data);

		render_data.model = translate(identity<mat4>(), vec3(0.f, -2.f, 0.f)) * scale(mat4(1.f), vec3(25.f, 0.5f, 25.f));
		mesh_draw(cube, render_data);

		render_data.model = translate(identity<mat4>(), vec3(3.5f, 0.f, 0.f));
		mesh_draw(ball, render_data);

		render_data.model = translate(identity<mat4>(), vec3(-3.5f, 0.f, 0.f)) * rotate(mat4(1.f), Time, vec3(0.f, 1.f, 0.f));
		render_data.material = &mat2;
		mesh_draw(monkeyHead, render_data);

		for (int i = 0; i < 4; i++)
		{
			render_data.model = translate(identity<mat4>(), render_data.point_light[i].position) * scale(mat4(1.f), vec3(0.1f));
			render_data.material = &mat_pointlight;
			mesh_draw(ball, render_data);
		}


		glfwSwapBuffers(_window);
		glfwPollEvents();	
	}

}
