#pragma once
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"

extern GLFWwindow* _window;

extern float delta_Time;

extern glm::vec2 mouse_position;
extern glm::vec2 mouse_delta;

extern Camera cam;

extern bool cam_control;