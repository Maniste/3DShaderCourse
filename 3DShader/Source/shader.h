#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

const int MAX_TEXTURES = 4;

struct Material
{
	GLuint program;
	GLuint textures[MAX_TEXTURES];
	int texture_num = 0;
};

Material load_material(const char* vert_path, const char* frag_path);
void materia_add_texture(Material* mat, GLuint texture);

//helpers
void material_use(Material mat);
void material_set(const char* name, float value);
void material_set(const char* name, const glm::vec3& value);
void material_set(const char* name, const glm::mat4& value);