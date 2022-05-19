#include "mesh.h"
#include "wavefront.h"
#include <gl/glew.h>
#include "shader.h"

struct  Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normals;
};


Mesh mesh_load(const char* path)
{
	Wavefront_File* obj = wavefront_load(path);

	if (obj == nullptr)
	{
		printf("Failed to load mesh '%s', file doesn't exist :(\n", path);
		return Mesh();
	}

	// Build vertices from face data in OBJ file
	Vertex* verts = new Vertex[obj->num_triangles * 3];
	int num_verts = 0;

	for (int tri_idx = 0; tri_idx < obj->num_triangles; ++tri_idx)
	{
		Wavefront_Triangle& triangle = obj->triangles[tri_idx];
		for (int vert_idx = 0; vert_idx < 3; ++vert_idx)
		{
			Wavefront_Vertex& vertex = triangle.vertices[vert_idx];
			Vertex new_vertex;
			new_vertex.position = obj->positions[vertex.idx_position];
			new_vertex.normals = obj->normals[vertex.idx_normal];
			new_vertex.texcoord = obj->texcoords[vertex.idx_texcoord];
			verts[num_verts++] = new_vertex;
		}
	}


	//Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_verts, verts, GL_STATIC_DRAW);

	//Bind attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2, GL_FLOAT, false, sizeof(Vertex), (void*)sizeof(glm::vec3));

	Mesh mesh;
	mesh.vao = vao;
	mesh.draw_count = num_verts;
	return mesh;
}

void mesh_draw(const Mesh& mesh, const Render_Data& data)
{
	glBindVertexArray(mesh.vao);

	//set material
	material_use(*data.material);

	//set matricies
	material_set("u_Model", data.model);
	material_set("u_ViewProjection", data.view_projection);

	//set lights
	material_set("u_DirectionalLight", data.directional_lght);
	material_set("u_EyePosition", data.eye_position);


	material_set("u_PointLight[0].position", data.point_light[0].position);
	material_set("u_PointLight[0].intensity", data.point_light[0].intensity);
	material_set("u_PointLight[0].color", data.point_light[0].color);


	material_set("u_PointLight[1].position", data.point_light[1].position);
	material_set("u_PointLight[1].intensity", data.point_light[1].intensity);
	material_set("u_PointLight[1].color", data.point_light[1].color);

	 
	glDrawArrays(GL_TRIANGLES, 0, mesh.draw_count);
}