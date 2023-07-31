#include <phoenix/window.h>

static GLFWwindow* gWindow;

void pxCreateWindow(I32 width, I32 height, const cstr_t title, bool startVisible) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glfwWindowHint(GLFW_VISIBLE, startVisible ? GLFW_TRUE : GLFW_FALSE);

	gWindow = glfwCreateWindow(width, height, title, NULL, NULL);


}

GLFWwindow* pxGetWindow() {
	return gWindow;
}


void pxShowWindow() {
	glfwShowWindow(gWindow);
}

