#pragma once
#include <glm/glm.hpp>

const float CAM_SENSITIVITY = 0.004f;
const float CAM_SPEED = 4.f;

struct Camera 
{
	float yaw;
	float pitch;

	glm::vec3 position = glm::vec3(0.f, 0.f, 5.f);


	bool perspetive = true;
	float aspect = 1.f;
	float fov = 60.f;

	float near = 0.1f;
	float far = 20.f;

	float ortho_height = 5.f;
};

void cam_update();
glm::mat4 cam_matrix();

glm::quat cam_quat();
glm::vec3 cam_forward();
glm::vec3 cam_right();