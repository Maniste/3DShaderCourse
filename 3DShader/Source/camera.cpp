#include "camera.h"
#include <glm/gtc/quaternion.hpp>
#include "global.h"
using namespace glm;

void cam_update()
{
	if (!cam_control)
		return;

	//rotation
	cam.yaw -= mouse_delta.x * CAM_SENSITIVITY;
	cam.pitch -= mouse_delta.y * CAM_SENSITIVITY;

	//Movement
	vec3 move = vec3(0.f);
	if (glfwGetKey(_window, GLFW_KEY_D))
		move += cam_right();
	if (glfwGetKey(_window, GLFW_KEY_A))
		move -= cam_right();

	if (glfwGetKey(_window, GLFW_KEY_W))
		move += cam_forward();
	if (glfwGetKey(_window, GLFW_KEY_S))
		move -= cam_forward();

	if (glfwGetKey(_window, GLFW_KEY_SPACE))
		move += vec3(0.f, 1.f, 0.f);
	if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL))
		move += vec3(0.f, -1.f, 0.f);


	cam.position += move * CAM_SPEED * delta_Time;
}

glm::mat4 cam_matrix()
{
	mat4 view_matrix = lookAt(cam.position, cam.position + cam_forward(), vec3(0.f, 1.f, 0.f));
	mat4 proj_matrix;

	if (cam.perspetive)
		proj_matrix = perspective(radians(cam.fov), cam.aspect, cam.near, cam.far);
	else
	{
		float half_height = cam.ortho_height / 2.f;
		proj_matrix = ortho(-half_height * cam.aspect, half_height * cam.aspect, -half_height, half_height, cam.near, cam.far);
	}

	return proj_matrix * view_matrix;

}

glm::quat cam_quat()
{
	return angleAxis(cam.yaw, vec3(0.f, 1.f, 0.f)) * angleAxis(cam.pitch, vec3(1.f, 0.f, 0.f));
}

glm::vec3 cam_forward()
{
	return cam_quat() * vec3(0.f, 0.f, -1.f);
}

glm::vec3 cam_right()
{
	return cam_quat() * vec3(1.f, 0.f, 0.f);
}
