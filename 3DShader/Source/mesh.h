#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Material;

struct Point_Light
{
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 color = glm::vec3(1.f);
	float intensity = 1.f;
};

struct Render_Data 
{
	glm::mat4 view_projection = glm::identity<glm::mat4>();
	glm::mat4 model = glm::identity<glm::mat4>();
	
	Material* material = nullptr;;

	//light
	glm::vec3 directional_lght;
	glm::vec3 eye_position;

	Point_Light point_light[4];
};

struct Mesh
{
	GLuint vao;
	int draw_count;
};

Mesh mesh_load(const char* path);
void mesh_draw(const Mesh& mesh, const Render_Data& data);
