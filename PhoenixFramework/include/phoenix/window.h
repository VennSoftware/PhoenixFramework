#ifndef PHNX_WINDOW_H
#define PHNX_WINDOW_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <phoenix/phoenix.h>

void pxCreateWindow(I32 width, I32 height, const cstr_t title, bool startVisible);
GLFWwindow* pxGetWindow();

void pxShowWindow();

#endif // !PHNX_WINDOW_H
