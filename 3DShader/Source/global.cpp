#include "global.h"

GLFWwindow* _window;
float delta_Time;

glm::vec2 mouse_position;
glm::vec2 mouse_delta;

Camera cam;
bool cam_control = false;